#include "compile_context.h"

#include "resources.h"
#include "device.h"

#include <engine/log.h>

namespace gapi::vulkan
{
  void CompileContext::compileCommand(const BeginRenderPassCmd& cmd)
  {
    m_CurrentCmdBuf = m_Device->allocateGraphicsCmdBuffer();
    m_CurrentCmdBuf.begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    );

    vk::RenderPass rp = m_RenderPassStorage.GetRenderPass(cmd);
    vk::UniqueFramebuffer fbUnique = createFramebuffer(cmd, rp);
    vk::Framebuffer fb = *fbUnique;

    vk::ClearValue clearValues[MAX_RENDER_TARGETS+1];
    for (size_t i = 0; i < MAX_RENDER_TARGETS; ++i)
      clearValues[i].setColor(vk::ClearColorValue(std::array<uint32_t,4>{0,0,0,0}));
    clearValues[MAX_RENDER_TARGETS].setDepthStencil(vk::ClearDepthStencilValue(0,0));

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = rp;
    rpBeginInfo.framebuffer = fb;
    rpBeginInfo.clearValueCount = MAX_RENDER_TARGETS+1;
    rpBeginInfo.pClearValues = clearValues;

    m_CurrentCmdBuf.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);
    m_CurrentRenderPass = rp;
    m_CurrentSubpass = 0;

    GetCurrentFrameOwnedResources().m_Framebuffers.push_back(std::move(fbUnique));
  }

  vk::UniqueFramebuffer CompileContext::createFramebuffer(const BeginRenderPassCmd& cmd, const vk::RenderPass& rp)
  {
    vk::ImageView attachments[MAX_RENDER_TARGETS + 1];
    size_t attachmentsCount = 0;

    for (;attachmentsCount < MAX_RENDER_TARGETS; ++attachmentsCount)
    {
      const auto& rt = cmd.renderTargets[attachmentsCount];
      if (rt.texture != TextureHandler::Invalid)
        attachments[attachmentsCount] = m_Device->getImageView(rt.texture);
      else
        break;
    }

    if (cmd.depthStencil.texture != TextureHandler::Invalid)
      attachments[attachmentsCount] = m_Device->getImageView(cmd.depthStencil.texture);

    const auto surfaceExtent = m_Device->getSurfaceExtent();
    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = rp;
    fbCi.attachmentCount = attachmentsCount;
    fbCi.pAttachments = attachments;
    fbCi.setWidth(surfaceExtent.width);
    fbCi.setHeight(surfaceExtent.height);
    fbCi.layers = 1;

    return m_Device->m_Device->createFramebufferUnique(fbCi);
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
  }

  void CompileContext::compileCommand(const DrawCmd& cmd)
  {
    m_CurrentCmdBuf.draw(cmd.vertexCount, cmd.instanceCount, cmd.firstVertex, cmd.firstInstance);
  }
}
