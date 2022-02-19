#include "compile_context.h"

#include "device.h"
#include "gapi_to_vk.h"
#include "resources.h"

#include <engine/log.h>

namespace gapi::vulkan
{
  void CompileContext::compileCommand(const BeginRenderPassCmd& cmd)
  {
    m_CurrentCmdBuf = m_Device->AllocateGraphicsCmdBuffer();
    UpdateViewport(cmd);

    vk::RenderPass rp = m_RenderPassStorage.GetRenderPass(cmd);
    vk::UniqueFramebuffer fbUnique = createFramebuffer(cmd, rp);
    vk::Framebuffer fb = *fbUnique;

    std::array<uint32_t,4> clearColor{0,0,0,0};
    Utils::FixedStack<vk::ClearValue, MAX_RENDER_TARGETS+1> clearValues;
    for (size_t i = 0; i < MAX_RENDER_TARGETS; ++i)
      clearValues.Push(vk::ClearValue().setColor(vk::ClearColorValue(clearColor)));

    if (cmd.depthStencil.texture != TextureHandler::Invalid)
      clearValues.Push(vk::ClearValue().setDepthStencil(vk::ClearDepthStencilValue(0,0)));

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = rp;
    rpBeginInfo.framebuffer = fb;
    rpBeginInfo.clearValueCount = clearValues.GetSize();
    rpBeginInfo.pClearValues = clearValues.GetData();
    rpBeginInfo.renderArea = vk::Rect2D{ {0,0}, m_CurrentViewportDim };

    m_CurrentCmdBuf.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);
    m_CurrentRenderPass = rp;
    m_CurrentSubpass = 0;

    GetCurrentFrameOwnedResources().m_Framebuffers.push_back(std::move(fbUnique));

  }

  vk::UniqueFramebuffer CompileContext::createFramebuffer(const BeginRenderPassCmd& cmd, const vk::RenderPass& rp)
  {
    Utils::FixedStack<vk::ImageView, MAX_RENDER_TARGETS + 1> attachments;
    size_t attachmentsCount = 0;

    for(const auto& rt: cmd.renderTargets)
      attachments.Push(m_Device->getImageView(rt.texture));

    if (cmd.depthStencil.texture != TextureHandler::Invalid)
      attachments.Push(m_Device->getImageView(cmd.depthStencil.texture));

    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = rp;
    fbCi.attachmentCount = attachments.GetSize();
    fbCi.pAttachments = attachments.GetData();
    fbCi.setWidth(m_CurrentViewportDim.width);
    fbCi.setHeight(m_CurrentViewportDim.height);
    fbCi.layers = 1;

    return m_Device->m_Device->createFramebufferUnique(fbCi);
  }

  void CompileContext::UpdateViewport(const BeginRenderPassCmd& cmd)
  {
    vk::Extent2D min = {(uint32_t)~(0), (uint32_t)~(0)};
    for(const auto& rt: cmd.renderTargets)
    {
      if (rt.texture != TextureHandler::Invalid)
      {
        vk::Extent3D dim = m_Device->GetImageDim(rt.texture);
        min.width  = dim.width  < min.width  ? dim.width  : min.width;
        min.height = dim.height < min.height ? dim.height : min.height;
      }
      else
        break;
    }

    if (m_CurrentViewportDim != min)
    {
      m_CurrentViewportDim = min;
    }
  }

  void CompileContext::compileCommand(const EndRenderPassCmd& cmd)
  {
    m_CurrentCmdBuf.endRenderPass();
    m_CurrentCmdBuf.end();
    m_Device->SubmitGraphicsCmdBuf(m_CurrentCmdBuf);
  }

  void CompileContext::compileCommand(const BindGraphicsPipelineCmd& cmd)
  {
    vk::Pipeline pipeline = m_PipelinesStorage.GetPipeline(cmd.description, m_CurrentRenderPass, m_CurrentSubpass);
    m_CurrentCmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

    vk::Viewport vp;
    vp.x = 0;
    vp.y = 0;
    vp.width = m_CurrentViewportDim.width;
    vp.height = m_CurrentViewportDim.height;
    vp.minDepth = 0;
    vp.maxDepth = 1;
    m_CurrentCmdBuf.setViewport(0, 1, &vp);

    vk::Rect2D sc {{0,0}, m_CurrentViewportDim};
    m_CurrentCmdBuf.setScissor(0, 1, &sc);

    m_CurrentPipelineStages = cmd.description.shaderNames;
  }

  void CompileContext::compileCommand(const DrawCmd& cmd)
  {
    m_CurrentCmdBuf.draw(cmd.vertexCount, cmd.instanceCount, cmd.firstVertex, cmd.firstInstance);
  }

  void CompileContext::compileCommand(const PresentSurfaceImageCmd& cmd)
  {
    m_Device->PresentSurfaceImage();
  }

  void CompileContext::compileCommand(const PushConstantsCmd& cmd)
  {
    vk::PipelineLayout layout = m_PipelinesStorage.GetPipelineLayout(m_CurrentPipelineStages);
    if (layout == vk::PipelineLayout{})
    {
      logerror("vulkan: can't push constants: pipeline layout not found.");
      return;
    }

    vk::ShaderStageFlagBits stages = GetShaderStage(cmd.stage);
    m_CurrentCmdBuf.pushConstants(layout, stages, 0 , cmd.size, cmd.data);
  }

  void CompileContext::compileCommand(const BindVertexBufferCmd& cmd)
  {
    const vk::Buffer buffer = m_Device->GetBuffer(cmd.buffer);
    vk::DeviceSize offsets = {0};
    m_CurrentCmdBuf.bindVertexBuffers(0, 1, &buffer, &offsets);
  }

  void CompileContext::compileCommand(const BindIndexBufferCmd& cmd)
  {
    const vk::Buffer buffer = m_Device->GetBuffer(cmd.buffer);
    m_CurrentCmdBuf.bindIndexBuffer(buffer, 0, vk::IndexType::eUint32);
  }

  void CompileContext::compileCommand(const DrawIndexedCmd& cmd)
  {
    m_CurrentCmdBuf.drawIndexed(cmd.indexCount, cmd.instanceCount, cmd.firstIndex, cmd.vertexOffset, cmd.firstInstance);
  }

  void CompileContext::NextFrame()
  {
    m_CurrentFrame = (m_CurrentFrame + 1) % SWAPCHAIN_IMAGES_COUNT;
    GetCurrentFrameOwnedResources().Clear();
  }
}
