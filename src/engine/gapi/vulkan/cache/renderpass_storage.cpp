#include "renderpass_storage.h"

#include <engine/gapi/vulkan/device.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

#include <boost/functional/hash.hpp>

namespace gapi::vulkan
{
  static size_t hash_renderpass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing)
  {
    using boost::hash_combine;
    size_t hash = 0;

    for(const auto& rt: renderTargets)
      hash_combine(hash, rt.hash());

    hash_combine(hash, depthStencil.hash());
    hash_combine(hash, clearing);

    return hash;
  }

  vk::RenderPass RenderPassStorage::getRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing)
  {
    const size_t hash = hash_renderpass(renderTargets, depthStencil, clearing);

    const auto it = m_RenderPasses.find(hash);
    if (it != m_RenderPasses.end())
      return it->second.get();

    auto rpUnique = createRenderPass(renderTargets, depthStencil, clearing);
    auto rp = rpUnique.get();
    m_RenderPasses.insert({
      hash,
      std::move(rpUnique)
    });

    return rp;
  }

  vk::UniqueRenderPass RenderPassStorage::createRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing)
  {
    Utils::FixedStack<vk::AttachmentDescription, MAX_RENDER_TARGETS + 1> attachments;
    Utils::FixedStack<vk::AttachmentReference, MAX_RENDER_TARGETS + 1> attachmentsRef;

    const vk::AttachmentLoadOp rtLoad      = (clearing & CLEAR_RT)      ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp depthLoad   = (clearing & CLEAR_DEPTH)   ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp stencilLoad = (clearing & CLEAR_STENCIL) ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;

    size_t attachmentsCount = 0;
    for(const auto& rt: renderTargets)
    {
      attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(rt.texture) )
        .setLoadOp( rtLoad )
        .setStoreOp( vk::AttachmentStoreOp::eStore )
        .setInitialLayout( get_image_layout_for_texture_state(rt.initialState) )
        .setFinalLayout( get_image_layout_for_texture_state(rt.finalState) )
      );

      attachmentsRef.push(vk::AttachmentReference{}
        .setAttachment(attachmentsCount)
        .setLayout( get_image_layout_for_texture_state(rt.initialState))
      );

      attachmentsCount += 1;
    }
    size_t rtCount = attachmentsCount;

    if (rtCount == 0)
      logerror("BeginRenderPassCmd: render targets count: 0");

    size_t depthStencilId = -1;
    if (depthStencil.texture !=  TextureHandler::Invalid)
    {
      depthStencilId = attachmentsCount++;

      attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(depthStencil.texture) )
        .setLoadOp(depthLoad)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(stencilLoad)
        .setStencilStoreOp(vk::AttachmentStoreOp::eStore)
        .setInitialLayout( get_image_layout_for_texture_state(depthStencil.initialState) )
        .setFinalLayout( get_image_layout_for_texture_state(depthStencil.finalState) )
      );

      attachmentsRef.push(vk::AttachmentReference{}
        .setAttachment(depthStencilId)
        .setLayout( get_image_layout_for_texture_state(depthStencil.initialState))
      );
    }

    auto subpassDesc = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpassDesc.colorAttachmentCount = rtCount;
    subpassDesc.pColorAttachments = attachmentsRef.getData();

    if (depthStencil.texture !=  TextureHandler::Invalid)
      subpassDesc.pDepthStencilAttachment = &attachmentsRef.getLast();

    auto rpCi = vk::RenderPassCreateInfo{};
    rpCi.attachmentCount = attachmentsCount;
    rpCi.pAttachments = attachments.getData();
    rpCi.subpassCount = 1;
    rpCi.pSubpasses = &subpassDesc;


    return m_Device->m_Device->createRenderPassUnique(rpCi);
  }
}
