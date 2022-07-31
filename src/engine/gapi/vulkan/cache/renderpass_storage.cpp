#include "renderpass_storage.h"

#include <engine/gapi/vulkan/device.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

#include <boost/functional/hash.hpp>

namespace gapi::vulkan
{
  vk::RenderPass RenderPassStorage::getRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing)
  {
    ASSERT_FMT(renderTargets.getSize() > 0, "At least one RT has to be provided");

    const size_t hash = hashRenderPass(renderTargets, depthStencil, clearing);

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

  size_t RenderPassStorage::hashRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing) const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, clearing);

    const auto hashAttachment = [this](const RenderPassAttachment& att)
    {
      size_t hash = 0;
      const auto format = m_Device->getTextureFormat(att.texture);
      hash_combine(hash, format);
      hash_combine(hash, att.initialState);
      hash_combine(hash, att.finalState);

      return hash;
    };

    for (const auto& rt: renderTargets)
    {
      size_t rtHash = hashAttachment(rt);
      hash_combine(hash, rtHash);
    }

    if (depthStencil.texture !=  TextureHandler::Invalid)
    {
      size_t rtHash = hashAttachment(depthStencil);
      hash_combine(hash, rtHash);
    }

    return hash;
  }

  RenderPassStorage::CreateInfo RenderPassStorage::getCreateInfo(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing) const
  {
    CreateInfo ret;

    const vk::AttachmentLoadOp rtLoad      = (clearing & CLEAR_RT)      ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp depthLoad   = (clearing & CLEAR_DEPTH)   ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;
    const vk::AttachmentLoadOp stencilLoad = (clearing & CLEAR_STENCIL) ?  vk::AttachmentLoadOp::eClear :  vk::AttachmentLoadOp::eLoad;

    size_t attachmentsCount = 0;
    for(const auto& rt: renderTargets)
    {
      ret.attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(rt.texture) )
        .setLoadOp( rtLoad )
        .setStoreOp( vk::AttachmentStoreOp::eStore )
        .setInitialLayout( get_image_layout_for_texture_state(rt.initialState) )
        .setFinalLayout( get_image_layout_for_texture_state(rt.finalState) )
      );

      ret.attachmentsRef.push(vk::AttachmentReference{}
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

      ret.attachments.push(vk::AttachmentDescription{}
        .setFormat( m_Device->getTextureFormat(depthStencil.texture) )
        .setLoadOp(depthLoad)
        .setStoreOp(vk::AttachmentStoreOp::eStore)
        .setStencilLoadOp(stencilLoad)
        .setStencilStoreOp(vk::AttachmentStoreOp::eStore)
        .setInitialLayout( get_image_layout_for_texture_state(depthStencil.initialState) )
        .setFinalLayout( get_image_layout_for_texture_state(depthStencil.finalState) )
      );

      ret.attachmentsRef.push(vk::AttachmentReference{}
        .setAttachment(depthStencilId)
        .setLayout( get_image_layout_for_texture_state(depthStencil.initialState))
      );
    }

    ret.subpassDesc.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    ret.subpassDesc.colorAttachmentCount = rtCount;
    ret.subpassDesc.pColorAttachments = ret.attachmentsRef.getData();

    if (depthStencil.texture !=  TextureHandler::Invalid)
      ret.subpassDesc.pDepthStencilAttachment = &ret.attachmentsRef.getLast();

    ret.rpCi.attachmentCount = attachmentsCount;
    ret.rpCi.pAttachments = ret.attachments.getData();
    ret.rpCi.subpassCount = 1;
    ret.rpCi.pSubpasses = &ret.subpassDesc;

    return ret;
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

    auto rp = m_Device->m_Device->createRenderPassUnique(rpCi);
    VK_CHECK_RES(rp);
    return std::move(rp.value);
  }
}
