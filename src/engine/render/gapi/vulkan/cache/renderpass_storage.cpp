#include "renderpass_storage.h"

#include <engine/render/gapi/vulkan/device.h>
#include <engine/log.h>

#include <boost/functional/hash.hpp>

namespace gapi::vulkan
{
  static size_t HashRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing)
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(const auto& rt: renderTargets)
      hash_combine(hash, rt);

    hash_combine(hash, depthStencil);
    hash_combine(hash, clearing);

    return hash;
  }

  vk::RenderPass RenderPassStorage::GetRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing)
  {
    const size_t hash = HashRenderPass(renderTargets, depthStencil, clearing);

    const auto it = m_RenderPasses.find(hash);
    if (it != m_RenderPasses.end())
      return it->second.get();

    auto rpUnique = CreateRenderPass(renderTargets, depthStencil, clearing);
    auto rp = rpUnique.get();
    m_RenderPasses.insert({
      hash,
      std::move(rpUnique)
    });

    return rp;
  }

   vk::UniqueRenderPass RenderPassStorage::CreateRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing)
  {
    Utils::FixedStack<vk::AttachmentDescription, MAX_RENDER_TARGETS + 1> attachments;
    Utils::FixedStack<vk::AttachmentReference, MAX_RENDER_TARGETS + 1> attachmentsRef;

    const vk::AttachmentLoadOp rtLoad      = (clearing & CLEAR_RT)      ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp depthLoad   = (clearing & CLEAR_DEPTH)   ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp stencilLoad = (clearing & CLEAR_STENCIL) ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;

    size_t attachmentsCount = 0;
    for(const auto& rt: renderTargets)
    {
      attachments.Push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(rt) )
        .setLoadOp( rtLoad )
        .setStoreOp( vk::AttachmentStoreOp::eStore )
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

    //size_t depthStencilId = -1;
    //if (depthStencil !=  TextureHandler::Invalid)
    //{
    //  depthStencilId = ++attachmentsCount;
    //
    //  vk::ImageLayout layout = vk::ImageLayout::eUndefined;
    //  bool hasDepth = att.depthLoadOp != TextureLoadOp::DontCare && att.depthLoadOp != TextureLoadOp::DontCare;
    //  bool hasStencil = att.stencilLoadOp != TextureLoadOp::DontCare && att.stencilLoadOp != TextureLoadOp::DontCare;
    //
    //  if (hasDepth && hasStencil)
    //    layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
    //  else if (hasDepth)
    //    layout = vk::ImageLayout::eDepthAttachmentOptimal;
    //  else
    //    layout = vk::ImageLayout::eStencilAttachmentOptimal;
    //
    //  attachments.Push(vk::AttachmentDescription{}
    //    .setFormat( m_Device->getTextureFormat(att.texture) )
    //    .setLoadOp( loadOpToVk(att.depthLoadOp) )
    //    .setStoreOp( storeOpToVk(att.depthStoreOp) )
    //    .setStencilLoadOp( loadOpToVk(att.stencilLoadOp) )
    //    .setStencilStoreOp( storeOpToVk(att.stencilStoreOp) )
    //    .setInitialLayout( layout )
    //    .setFinalLayout( layout )
    //  );
    //}

    auto subpassDesc = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpassDesc.colorAttachmentCount = rtCount;
    subpassDesc.pColorAttachments = attachmentsRef.GetData();

    if (depthStencil !=  TextureHandler::Invalid)
      subpassDesc.pDepthStencilAttachment = &attachmentsRef.GetLast();

    auto rpCi = vk::RenderPassCreateInfo{};
    rpCi.attachmentCount = attachmentsCount;
    rpCi.pAttachments = attachments.GetData();
    rpCi.subpassCount = 1;
    rpCi.pSubpasses = &subpassDesc;


    return m_Device->m_Device->createRenderPassUnique(rpCi);
  }
}
