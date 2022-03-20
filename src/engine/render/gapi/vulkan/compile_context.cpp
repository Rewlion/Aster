#include "compile_context.h"

#include "device.h"
#include "gapi_to_vk.h"
#include "resources.h"

#include <engine/log.h>

namespace gapi::vulkan
{
  void CompileContext::compileCommand(const BeginRenderPassCmd& cmd)
  {
    const vk::Extent2D min = GetMinRenderSize(cmd.renderTargets, cmd.depthStencil);
    const vk::Rect2D renderArea = vk::Rect2D{{0,0}, min};

    m_State.renderPassState.Set<RenderPassTSF, RenderTargets>(cmd.renderTargets);
    m_State.renderPassState.Set<RenderPassTSF, TextureHandler>(cmd.depthStencil);
    m_State.renderPassState.Set<RenderPassTSF, vk::Rect2D>(renderArea);
  }

  vk::RenderPass CompileContext::GetRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing)
  {
    return m_RenderPassStorage.GetRenderPass(renderTargets, depthStencil, clearing);
  }

  vk::Framebuffer CompileContext::GetFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    vk::UniqueFramebuffer fb = CreateFramebuffer(renderArea, renderTargets, depthStencil);
    const auto framebuffer = fb.get();
    GetCurrentFrameOwnedResources().m_Framebuffers.push_back(std::move(fb));

    return framebuffer;
  }

  vk::Pipeline CompileContext::GetPipeline(const GraphicsPipelineDescription& desc)
  {
    return m_PipelinesStorage.GetPipeline(desc, m_State.renderPass, 0);
  }

  bool CompileContext::GetPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout)
  {
    return m_PipelinesStorage.GetPipelineLayout(stageNames, layout);
  }

  void CompileContext::UpdateDescriptorSets()
  {
    InsureActiveCmd();
    GetCurrentFrameOwnedResources().m_DescriptorSetsManager.SetPipelineLayout(m_State.layout);
    GetCurrentFrameOwnedResources().m_DescriptorSetsManager.UpdateDescriptorSets(m_State.cmdBuffer);
  }

  void CompileContext::EndRenderPass(const char* why)
  {
    if (m_State.renderPass != vk::RenderPass{})
    {
      m_State.cmdBuffer.endRenderPass();
      SubmitGraphicsCmd();
      m_State.renderPass = vk::RenderPass{};

      m_State.renderPassState.MarkDirty<RenderPassTSF>();
      m_State.graphicsState.MarkDirty<GraphicsPipelineTSF>();
      m_State.graphicsState.MarkDirty<VertexBufferTSF>();
      m_State.graphicsState.MarkDirty<IndexBufferTSF>();
      m_State.graphicsState.MarkDirty<PushConstantTSF>();
    }
  }

  void CompileContext::SubmitGraphicsCmd()
  {
    m_State.cmdBuffer.end();
    m_Device->SubmitGraphicsCmdBuf(m_State.cmdBuffer);

    m_State.cmdBuffer = vk::CommandBuffer{};
  }

  void CompileContext::InsureActiveCmd()
  {
    if (m_State.cmdBuffer == vk::CommandBuffer{})
      m_State.cmdBuffer = m_Device->AllocateGraphicsCmdBuffer();
  }

  void CompileContext::FlushGraphicsState()
  {
    m_State.renderPassState.Apply(*this, m_State);
    m_State.graphicsState.Apply(*this, m_State);
  }

  vk::UniqueFramebuffer CompileContext::CreateFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    Utils::FixedStack<vk::ImageView, MAX_RENDER_TARGETS + 1> attachments;
    size_t attachmentsCount = 0;

    for(const auto& rt: renderTargets)
      attachments.Push(m_Device->getImageView(rt));

    if (depthStencil != TextureHandler::Invalid)
      attachments.Push(m_Device->getImageView(depthStencil));

    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = m_State.renderPass;
    fbCi.attachmentCount = attachments.GetSize();
    fbCi.pAttachments = attachments.GetData();
    fbCi.setWidth(renderArea.width);
    fbCi.setHeight(renderArea.height);
    fbCi.layers = 1;

    return m_Device->m_Device->createFramebufferUnique(fbCi);
  }

  vk::Extent2D CompileContext::GetMinRenderSize(const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    vk::Extent2D min = {(uint32_t)~(0), (uint32_t)~(0)};
    for(const auto& rt: renderTargets)
    {
      if (rt!= TextureHandler::Invalid)
      {
        vk::Extent3D dim = m_Device->GetImageDim(rt);
        min.width  = dim.width  < min.width  ? dim.width  : min.width;
        min.height = dim.height < min.height ? dim.height : min.height;
      }
    }

    if ((min != vk::Extent2D{0,0}) && (depthStencil != TextureHandler::Invalid))
    {
      vk::Extent3D dim = m_Device->GetImageDim(depthStencil);
      min.width  = dim.width  < min.width  ? dim.width  : min.width;
      min.height = dim.height < min.height ? dim.height : min.height;
    }

    return min;
  }

  void CompileContext::compileCommand(const BindGraphicsPipelineCmd& cmd)
  {
    m_State.graphicsState.Set<GraphicsPipelineTSF, ShaderStagesNames>(cmd.shaderNames);
    m_State.graphicsState.Set<GraphicsPipelineTSF, PrimitiveTopology>(cmd.topology);
    m_State.graphicsState.Set<GraphicsPipelineTSF, DepthStencilStateDescription>(cmd.depthStencilState);
  }

  void CompileContext::compileCommand(const DrawCmd& cmd)
  {
    m_State.graphicsState.Set<VertexBufferTSF, bool>(true);
    m_State.graphicsState.Set<IndexBufferTSF, bool>(false);

    FlushGraphicsState();

    m_State.cmdBuffer.draw(cmd.vertexCount, cmd.instanceCount, cmd.firstVertex, cmd.firstInstance);
  }

  void CompileContext::compileCommand(const PresentSurfaceImageCmd& cmd)
  {
    InsureActiveCmd();
    EndRenderPass("Swap backbuffer");

    m_Device->PresentSurfaceImage();
  }

  void CompileContext::compileCommand(const PushConstantsCmd& cmd)
  {
    vk::ShaderStageFlagBits stages = GetShaderStage(cmd.stage);

    m_State.graphicsState.Set<PushConstantTSF, vk::ShaderStageFlagBits>(stages);
    m_State.graphicsState.Set<PushConstantTSF, size_t>(cmd.size);
    m_State.graphicsState.Set<PushConstantTSF, const void*>(cmd.data);

    m_State.graphicsState.MarkDirty<PushConstantTSF>();
  }

  void CompileContext::compileCommand(const BindVertexBufferCmd& cmd)
  {
    const vk::Buffer buffer = m_Device->GetBuffer(cmd.buffer);
    m_State.graphicsState.Set<VertexBufferTSF, vk::Buffer>(buffer);
  }

  void CompileContext::compileCommand(const BindIndexBufferCmd& cmd)
  {
    const vk::Buffer buffer = m_Device->GetBuffer(cmd.buffer);
    m_State.graphicsState.Set<IndexBufferTSF, vk::Buffer>(buffer);
  }

  void CompileContext::compileCommand(const DrawIndexedCmd& cmd)
  {
    m_State.graphicsState.Set<VertexBufferTSF, bool>(true);
    m_State.graphicsState.Set<IndexBufferTSF, bool>(true);

    InsureActiveCmd();
    FlushGraphicsState();

    m_State.cmdBuffer.drawIndexed(cmd.indexCount, cmd.instanceCount, cmd.firstIndex, cmd.vertexOffset, cmd.firstInstance);
  }

  void CompileContext::compileCommand(const BindTextureCmd& cmd)
  {
    auto& dsManager = GetCurrentFrameOwnedResources().m_DescriptorSetsManager;

    if (cmd.texture != TextureHandler::Invalid)
    {
      InsureActiveCmd();
      EndRenderPass("Bind texture");

      m_Device->ImageBarrier(m_State.cmdBuffer, cmd.texture, vk::ImageLayout::eShaderReadOnlyOptimal,
                             vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eFragmentShader);

      vk::ImageView imgView = m_Device->getImageView(cmd.texture);
      dsManager.SetImage(imgView, cmd.argument, cmd.binding);

      m_State.graphicsState.MarkDirty<FlushDescriptorSetsTSF>();
    }
  }

  void CompileContext::compileCommand(const BindSamplerCmd& cmd)
  {
    auto& dsManager = GetCurrentFrameOwnedResources().m_DescriptorSetsManager;

    if (cmd.sampler != SamplerHandler::Invalid)
    {
      vk::Sampler sampler = m_Device->GetSampler(cmd.sampler);
      dsManager.SetSampler(sampler, cmd.argument, cmd.binding);

      m_State.graphicsState.MarkDirty<FlushDescriptorSetsTSF>();
    }
  }

  void CompileContext::compileCommand(const ClearCmd& cmd)
  {
    m_State.renderPassState.Set<RenderPassTSF, ClearState>(cmd.clearing);
  }

  void CompileContext::NextFrame()
  {
    m_CurrentFrame = (m_CurrentFrame + 1) % SWAPCHAIN_IMAGES_COUNT;
    GetCurrentFrameOwnedResources().Clear();
  }

  void CompileContext::compileCommand(const SetBlendStateCmd& cmd)
  {
    m_State.graphicsState.Set<GraphicsPipelineTSF, BlendState>(cmd.blending);
  }
}
