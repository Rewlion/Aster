#include "renderpass_storage.h"

#include <engine/render/gapi/vulkan/device.h>
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
    Utils::FixedStack<vk::AttachmentDescription, MAX_RENDER_TARGETS + 1> attachments;
    Utils::FixedStack<vk::AttachmentReference, MAX_RENDER_TARGETS + 1> attachmentsRef;

    size_t attachmentsCount = 0;
    for(const auto& cmdRt: cmd.renderTargets)
    {
      attachments.Push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(cmdRt.texture) )
        .setLoadOp( loadOpToVk(cmdRt.loadOp) )
        .setStoreOp( storeOpToVk(cmdRt.storeOp) )
        .setInitialLayout( vk::ImageLayout::eUndefined )
        .setFinalLayout( vk::ImageLayout::eColorAttachmentOptimal )
      );

      attachmentsRef.Push(vk::AttachmentReference{}
        .setAttachment(attachmentsCount)
        .setLayout(vk::ImageLayout::eColorAttachmentOptimal)
      );

      attachmentsCount += 1;
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

      attachments.Push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(att.texture) )
        .setLoadOp( loadOpToVk(att.depthLoadOp) )
        .setStoreOp( storeOpToVk(att.depthStoreOp) )
        .setStencilLoadOp( loadOpToVk(att.stencilLoadOp) )
        .setStencilStoreOp( storeOpToVk(att.stencilStoreOp) )
        .setInitialLayout( layout )
        .setFinalLayout( layout )
      );
    }

    auto subpassDesc = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpassDesc.colorAttachmentCount = rtCount;
    subpassDesc.pColorAttachments = attachmentsRef.GetData();

    if (cmd.depthStencil.texture !=  TextureHandler::Invalid)
      subpassDesc.pDepthStencilAttachment = &attachmentsRef.GetLast();

    auto rpCi = vk::RenderPassCreateInfo{};
    rpCi.attachmentCount = attachmentsCount;
    rpCi.pAttachments = attachments.GetData();
    rpCi.subpassCount = 1;
    rpCi.pSubpasses = &subpassDesc;


    return m_Device->m_Device->createRenderPassUnique(rpCi);
  }
}
