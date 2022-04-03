#include "compile_context.h"

#include "device.h"
#include "frame_gc.h"
#include "gapi_to_vk.h"
#include "resources.h"

#include <engine/log.h>

namespace gapi::vulkan
{
  void CompileContext::compileCommand(const BeginRenderPassCmd& cmd)
  {
    const vk::Extent2D min = getMinRenderSize(cmd.renderTargets, cmd.depthStencil);
    const vk::Rect2D renderArea = vk::Rect2D{{0,0}, min};

    m_State.renderPassState.set<RenderPassTSF, RenderTargets>(cmd.renderTargets);
    m_State.renderPassState.set<RenderPassTSF, TextureHandler>(cmd.depthStencil);
    m_State.renderPassState.set<RenderPassTSF, vk::Rect2D>(renderArea);
  }

  vk::RenderPass CompileContext::getRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing)
  {
    return m_RenderPassStorage.getRenderPass(renderTargets, depthStencil, clearing);
  }

  vk::Framebuffer CompileContext::getFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    vk::UniqueFramebuffer fb = createFramebuffer(renderArea, renderTargets, depthStencil);
    const auto framebuffer = fb.get();
    m_FrameGc->addFramebuffer(std::move(fb));

    return framebuffer;
  }

  vk::Pipeline CompileContext::getPipeline(const GraphicsPipelineDescription& desc)
  {
    return m_PipelinesStorage.getPipeline(desc, m_State.renderPass, 0);
  }

  bool CompileContext::getPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout)
  {
    return m_PipelinesStorage.getPipelineLayout(stageNames, layout);
  }

  void CompileContext::updateDescriptorSets()
  {
    insureActiveCmd();
    getCurrentFrameOwnedResources().m_DescriptorSetsManager.setPipelineLayout(m_State.layout);
    getCurrentFrameOwnedResources().m_DescriptorSetsManager.updateDescriptorSets(m_State.cmdBuffer);
  }

  void CompileContext::endRenderPass(const char* why)
  {
    if (m_State.renderPass != vk::RenderPass{})
    {
      m_State.cmdBuffer.endRenderPass();
      queueGraphicsCmd();
      m_State.renderPass = vk::RenderPass{};

      m_State.renderPassState.markDirty<RenderPassTSF>();
      m_State.graphicsState.markDirty<GraphicsPipelineTSF>();
      m_State.graphicsState.markDirty<VertexBufferTSF>();
      m_State.graphicsState.markDirty<IndexBufferTSF>();
      m_State.graphicsState.markDirty<PushConstantTSF>();
    }
  }

  void CompileContext::queueGraphicsCmd()
  {
    m_State.cmdBuffer.end();
    m_QueuedGraphicsCommands.push_back(m_State.cmdBuffer);
    m_State.cmdBuffer = vk::CommandBuffer{};
  }

  void CompileContext::insureActiveCmd()
  {
    if (m_State.cmdBuffer == vk::CommandBuffer{})
      m_State.cmdBuffer = m_Device->allocateGraphicsCmdBuffer();
  }

  void CompileContext::flushGraphicsState()
  {
    m_State.renderPassState.apply(*this, m_State);
    m_State.graphicsState.apply(*this, m_State);
  }

  vk::UniqueFramebuffer CompileContext::createFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    Utils::FixedStack<vk::ImageView, MAX_RENDER_TARGETS + 1> attachments;
    size_t attachmentsCount = 0;

    for(const auto& rt: renderTargets)
      attachments.push(m_Device->getImageView(rt));

    if (depthStencil != TextureHandler::Invalid)
      attachments.push(m_Device->getImageView(depthStencil));

    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = m_State.renderPass;
    fbCi.attachmentCount = attachments.getSize();
    fbCi.pAttachments = attachments.getData();
    fbCi.setWidth(renderArea.width);
    fbCi.setHeight(renderArea.height);
    fbCi.layers = 1;

    return m_Device->m_Device->createFramebufferUnique(fbCi);
  }

  vk::Extent2D CompileContext::getMinRenderSize(const RenderTargets& renderTargets, const TextureHandler depthStencil)
  {
    vk::Extent2D min = {(uint32_t)~(0), (uint32_t)~(0)};
    for(const auto& rt: renderTargets)
    {
      if (rt!= TextureHandler::Invalid)
      {
        vk::Extent3D dim = m_Device->getImageDim(rt);
        min.width  = dim.width  < min.width  ? dim.width  : min.width;
        min.height = dim.height < min.height ? dim.height : min.height;
      }
    }

    if ((min != vk::Extent2D{0,0}) && (depthStencil != TextureHandler::Invalid))
    {
      vk::Extent3D dim = m_Device->getImageDim(depthStencil);
      min.width  = dim.width  < min.width  ? dim.width  : min.width;
      min.height = dim.height < min.height ? dim.height : min.height;
    }

    return min;
  }

  void CompileContext::compileCommand(const BindGraphicsShadersCmd& cmd)
  {
    m_State.graphicsState.set<GraphicsPipelineTSF, ShaderStagesNames>(cmd.shaderNames);
  }

  void CompileContext::compileCommand(const PresentSurfaceImageCmd& cmd)
  {
    insureActiveCmd();
    endRenderPass("Swap backbuffer");

    prepareBackbufferForPresent();
    queueGraphicsCmd();
    submitGraphicsCmds();

    m_Device->presentSurfaceImage();

    nextFrame();
  }

  void CompileContext::compileCommand(const PushConstantsCmd& cmd)
  {
    vk::ShaderStageFlagBits stages = get_shader_stage(cmd.stage);

    m_State.graphicsState.set<PushConstantTSF, vk::ShaderStageFlagBits>(stages);
    m_State.graphicsState.set<PushConstantTSF, size_t>(cmd.size);
    m_State.graphicsState.set<PushConstantTSF, const void*>(cmd.data);

    m_State.graphicsState.markDirty<PushConstantTSF>();
  }

  void CompileContext::compileCommand(const BindVertexBufferCmd& cmd)
  {
    const Buffer& buffer = m_Device->getBuffer(cmd.buffer);
    m_State.graphicsState.set<VertexBufferTSF, vk::Buffer>(buffer.buffer.get());
  }

  void CompileContext::compileCommand(const BindIndexBufferCmd& cmd)
  {
    const Buffer& buffer = m_Device->getBuffer(cmd.buffer);
    m_State.graphicsState.set<IndexBufferTSF, vk::Buffer>(buffer.buffer.get());
  }

  void CompileContext::compileCommand(const DrawCmd& cmd)
  {
    m_State.graphicsState.set<GraphicsPipelineTSF, PrimitiveTopology>(cmd.topology);
    m_State.graphicsState.set<VertexBufferTSF, bool>(true);
    m_State.graphicsState.set<IndexBufferTSF, bool>(false);

    insureActiveCmd();
    flushGraphicsState();

    m_State.cmdBuffer.draw(cmd.vertexCount, cmd.instanceCount, cmd.firstVertex, cmd.firstInstance);
  }

  void CompileContext::compileCommand(const DrawIndexedCmd& cmd)
  {
    m_State.graphicsState.set<GraphicsPipelineTSF, PrimitiveTopology>(cmd.topology);
    m_State.graphicsState.set<VertexBufferTSF, bool>(true);
    m_State.graphicsState.set<IndexBufferTSF, bool>(true);

    insureActiveCmd();
    flushGraphicsState();

    m_State.cmdBuffer.drawIndexed(cmd.indexCount, cmd.instanceCount, cmd.firstIndex, cmd.vertexOffset, cmd.firstInstance);
  }

  void CompileContext::compileCommand(const BindConstantBufferCmd& cmd)
  {
    auto& dsManager = getCurrentFrameOwnedResources().m_DescriptorSetsManager;

    if (cmd.buffer != BufferHandler::Invalid)
    {
      const Buffer& buffer = m_Device->getBuffer(cmd.buffer);
      dsManager.setUniformBuffer(buffer.buffer.get(), cmd.argument, cmd.binding, 0, 0);

      m_State.graphicsState.markDirty<FlushDescriptorSetsTSF>();
    }
  }

  void CompileContext::compileCommand(const BindTextureCmd& cmd)
  {
    auto& dsManager = getCurrentFrameOwnedResources().m_DescriptorSetsManager;

    if (cmd.texture != TextureHandler::Invalid)
    {
      if (m_Device->getImageLayout(cmd.texture) != vk::ImageLayout::eShaderReadOnlyOptimal)
      {
        endRenderPass("Bind texture");
        imageBarrier(cmd.texture, vk::ImageLayout::eShaderReadOnlyOptimal,
                     vk::PipelineStageFlagBits::eFragmentShader,
                     vk::PipelineStageFlagBits::eFragmentShader);
      }

      vk::ImageView imgView = m_Device->getImageView(cmd.texture);
      dsManager.setImage(imgView, cmd.argument, cmd.binding);

      m_State.graphicsState.markDirty<FlushDescriptorSetsTSF>();
    }
  }

  void CompileContext::compileCommand(const BindSamplerCmd& cmd)
  {
    auto& dsManager = getCurrentFrameOwnedResources().m_DescriptorSetsManager;

    if (cmd.sampler != SamplerHandler::Invalid)
    {
      vk::Sampler sampler = m_Device->getSampler(cmd.sampler);
      dsManager.setSampler(sampler, cmd.argument, cmd.binding);

      m_State.graphicsState.markDirty<FlushDescriptorSetsTSF>();
    }
  }

  void CompileContext::compileCommand(const ClearCmd& cmd)
  {
    m_State.renderPassState.set<RenderPassTSF, ClearState>(cmd.clearing);
  }

  void CompileContext::compileCommand(const SetDepthStencilStateCmd& cmd)
  {
    m_State.graphicsState.set<GraphicsPipelineTSF, DepthStencilStateDescription>(cmd.ds);
  }

  void CompileContext::prepareBackbufferForPresent()
  {
    insureActiveCmd();

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    const auto layoutBarrier = vk::ImageMemoryBarrier()
      .setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
      .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
      .setImage(m_Device->getImage(m_Device->getBackbuffer()))
      .setSubresourceRange(subresourceRange);

    m_State.cmdBuffer.pipelineBarrier(
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);
  }

  void CompileContext::submitGraphicsCmds()
  {
    auto renderJobWaitFence = m_Device->submitGraphicsCmds(
      m_QueuedGraphicsCommands.data(), m_QueuedGraphicsCommands.size(),
      &m_BackbufferReadySemaphore, 1);

    m_QueuedGraphicsCommands.clear();

    m_RenderJobWaitFences[m_CurrentFrame].push_back(renderJobWaitFence.get());
    m_FrameGc->addFence(std::move(renderJobWaitFence));
  }

  void CompileContext::nextFrame()
  {
    m_CurrentFrame = (m_CurrentFrame + 1) % SWAPCHAIN_IMAGES_COUNT;

    if (!m_RenderJobWaitFences[m_CurrentFrame].empty())
    {
      m_Device->m_Device->waitForFences(m_RenderJobWaitFences[m_CurrentFrame].size(),
      m_RenderJobWaitFences[m_CurrentFrame].data(), true, -1);

      m_RenderJobWaitFences[m_CurrentFrame].clear();
    }

    getCurrentFrameOwnedResources().Clear();
    m_FrameGc->nextFrame();

    acquireBackbuffer();
  }

  void CompileContext::compileCommand(const SetBlendStateCmd& cmd)
  {
    m_State.graphicsState.set<GraphicsPipelineTSF, BlendState>(cmd.blending);
  }

  void CompileContext::imageBarrier(const TextureHandler handler, const vk::ImageLayout newLayout,
                        const vk::PipelineStageFlagBits srcStage, const vk::PipelineStageFlagBits dstStage)
  {
    insureActiveCmd();

    const vk::ImageLayout currentLayout = m_Device->getImageLayout(handler);

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    vk::ImageMemoryBarrier layoutBarrier;
    layoutBarrier.oldLayout = currentLayout;
    layoutBarrier.newLayout = newLayout;
    layoutBarrier.image = m_Device->getImage(handler);
    layoutBarrier.subresourceRange = subresourceRange;

    m_State.cmdBuffer.pipelineBarrier(
      srcStage,
      dstStage,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    m_Device->setImageLayout(handler, newLayout);
  }

  void CompileContext::acquireBackbuffer()
  {
    auto sem = m_Device->createSemaphore();
    m_BackbufferReadySemaphore = sem.get();
    m_FrameGc->addSemaphores(std::move(sem));
    m_Device->acquireBackbuffer(m_BackbufferReadySemaphore);
  }
}
