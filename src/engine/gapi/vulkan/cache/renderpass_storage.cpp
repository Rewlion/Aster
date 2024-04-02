#include "renderpass_storage.h"

#include <engine/gapi/vulkan/device.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

#include <boost/functional/hash.hpp>

namespace gapi::vulkan
{
  vk::RenderPass RenderPassStorage::getRenderPass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    const size_t hash = hashRenderPass(renderTargets, depthStencil);

    const auto it = m_RenderPasses.find(hash);
    if (it != m_RenderPasses.end())
      return it->second.get();

    auto rpUnique = createRenderPass(renderTargets, depthStencil);
    auto rp = rpUnique.get();
    m_RenderPasses.insert({
      hash,
      std::move(rpUnique)
    });

    return rp;
  }

  size_t RenderPassStorage::hashRenderPass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) const
  {
    ASSERT_FMT(renderTargets.hasAny() || depthStencil.texture != gapi::TextureHandle::Invalid,
                "vk: empty render pass");

    using boost::hash_combine;
    size_t hash = 0;

    const auto hashAttachment = [this](const RenderPassAttachment& att)
    {
      size_t hash = 0;
      const auto format = att.texture != gapi::TextureHandle::Invalid ?
                            m_Device->getTextureFormat(att.texture) :
                            vk::Format::eUndefined;
      hash_combine(hash, format);
      hash_combine(hash, att.initialState);
      hash_combine(hash, att.finalState);
      hash_combine(hash, att.loadOp);
      hash_combine(hash, att.storeOp);

      return hash;
    };

    const auto hashDepthStencilAttachment = [this](const RenderPassDepthStencilAttachment& att)
    {
      size_t hash = 0;
      const auto format = m_Device->getTextureFormat(att.texture);
      hash_combine(hash, format);
      hash_combine(hash, att.initialState);
      hash_combine(hash, att.finalState);
      hash_combine(hash, att.depthLoadOp);
      hash_combine(hash, att.depthStoreOp);
      hash_combine(hash, att.stencilLoadOp);
      hash_combine(hash, att.stencilStoreOp);

      return hash;
    };

    if (renderTargets.hasAny())
    {
      for (const auto& rt: renderTargets)
      {
        size_t rtHash = hashAttachment(rt);
        hash_combine(hash, rtHash);
      }
    }
    else
    {
      gapi::RenderPassAttachment emptyAttachment;
      emptyAttachment.texture = gapi::TextureHandle::Invalid;
      emptyAttachment.loadOp = gapi::LoadOp::DontCare;
      emptyAttachment.storeOp = gapi::StoreOp::DontCare;

      size_t rtHash = hashAttachment(emptyAttachment);
      hash_combine(hash, rtHash);
    }

    if (depthStencil.texture !=  TextureHandle::Invalid)
    {
      size_t rtHash = hashDepthStencilAttachment(depthStencil);
      hash_combine(hash, rtHash);
    }

    return hash;
  }

  vk::UniqueRenderPass RenderPassStorage::createRenderPass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    Utils::FixedStack<vk::AttachmentDescription, MAX_RENDER_TARGETS + 1> attachments;
    Utils::FixedStack<vk::AttachmentReference, MAX_RENDER_TARGETS + 1> attachmentsRef;

    size_t attachmentsCount = 0;
    for(const auto& rt: renderTargets)
    {
      attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(rt.texture) )
        .setLoadOp( get_load_op(rt.loadOp) )
        .setStoreOp( get_store_op(rt.storeOp) )
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

    size_t depthStencilId = -1;
    if (depthStencil.texture !=  TextureHandle::Invalid)
    {
      depthStencilId = attachmentsCount++;

      attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(depthStencil.texture) )
        .setLoadOp(get_load_op(depthStencil.depthLoadOp))
        .setStoreOp(get_store_op(depthStencil.depthStoreOp))
        .setStencilLoadOp(get_load_op(depthStencil.stencilLoadOp))
        .setStencilStoreOp(get_store_op(depthStencil.stencilStoreOp))
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

    if (depthStencil.texture !=  TextureHandle::Invalid)
      subpassDesc.pDepthStencilAttachment = &attachmentsRef.getLast();

    auto rpCi = vk::RenderPassCreateInfo{};
    rpCi.attachmentCount = attachmentsCount;
    rpCi.pAttachments = attachments.getData();
    rpCi.subpassCount = 1;
    rpCi.pSubpasses = &subpassDesc;

    auto rp = m_Device->m_Device->createRenderPassUnique(rpCi);
    VK_CHECK_RES(rp);
    return std::move(rp.value);
  }
}
