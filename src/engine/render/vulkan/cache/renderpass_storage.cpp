#include "renderpass_storage.h"

#include <engine/render/vulkan/device.h>
#include <engine/log.h>

namespace gapi::vulkan
{
  vk::RenderPass RenderPassStorage::GetRenderPass(const BeginRenderPassCmd& cmd)
  {
    const size_t hash = cmd.hash();

    const auto it = m_RenderPasses.find(hash);
    if (it != m_RenderPasses.end())
      return it->second.get();

    auto rpUnique = CreateRenderPass(cmd);
    auto rp = rpUnique.get();
    m_RenderPasses.insert({
      hash,
      std::move(rpUnique)
    });

    return rp;
  }

  vk::UniqueRenderPass RenderPassStorage::CreateRenderPass(const BeginRenderPassCmd& cmd)
  {
    vk::AttachmentDescription attachments[MAX_RENDER_TARGETS + 1];
    vk::AttachmentReference attachmentsRef[MAX_RENDER_TARGETS + 1];

    size_t attachmentsCount = 0;
    for (size_t i = 0; i < MAX_RENDER_TARGETS; ++i)
    {
      auto& cmdRt = cmd.renderTargets[i];
      if (cmdRt.texture != TextureHandler::Invalid)
      {
        attachments[i]
          .setFormat( m_Device->getTextureFormat(cmdRt.texture) )
          .setLoadOp( loadOpToVk(cmdRt.loadOp) )
          .setStoreOp( storeOpToVk(cmdRt.storeOp) )
          .setInitialLayout( vk::ImageLayout::eColorAttachmentOptimal )
          .setFinalLayout( vk::ImageLayout::eColorAttachmentOptimal );

        attachmentsRef[i]
          .setAttachment(i)
          .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        attachmentsCount += 1;
      }
      else
        break;
    }
    size_t rtCount = attachmentsCount;

    if (rtCount == 0)
      logerror("BeginRenderPassCmd: render targets count: 0");

    size_t depthStencilId = -1;
    if (cmd.depthStencil.texture !=  TextureHandler::Invalid)
    {

      depthStencilId = ++attachmentsCount;
      auto& att = cmd.depthStencil;

      vk::ImageLayout layout = vk::ImageLayout::eUndefined;
      bool hasDepth = att.depthLoadOp != TextureLoadOp::DontCare && att.depthLoadOp != TextureLoadOp::DontCare;
      bool hasStencil = att.stencilLoadOp != TextureLoadOp::DontCare && att.stencilLoadOp != TextureLoadOp::DontCare;

      if (hasDepth && hasStencil)
        layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
      else if (hasDepth)
        layout = vk::ImageLayout::eDepthAttachmentOptimal;
      else
        layout = vk::ImageLayout::eStencilAttachmentOptimal;

      attachments[depthStencilId]
        .setFormat( m_Device->getTextureFormat(att.texture) )
        .setLoadOp( loadOpToVk(att.depthLoadOp) )
        .setStoreOp( storeOpToVk(att.depthStoreOp) )
        .setStencilLoadOp( loadOpToVk(att.stencilLoadOp) )
        .setStencilStoreOp( storeOpToVk(att.stencilStoreOp) )
        .setInitialLayout( layout )
        .setFinalLayout( layout );
    }

    auto subpassDesc = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpassDesc.colorAttachmentCount = rtCount;
    subpassDesc.pColorAttachments = attachmentsRef;
    if (depthStencilId != -1)
      subpassDesc.pDepthStencilAttachment = &attachmentsRef[depthStencilId];

    auto rpCi = vk::RenderPassCreateInfo{};
    rpCi.attachmentCount = attachmentsCount;
    rpCi.pAttachments = attachments;
    rpCi.subpassCount = 1;
    rpCi.pSubpasses = &subpassDesc;


    return m_Device->m_Device->createRenderPassUnique(rpCi);
  }
}
