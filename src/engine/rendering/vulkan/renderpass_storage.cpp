#include "renderpass_storage.h"
#include "core.h"

namespace vk
{
  bool operator<(const vk::AttachmentReference& l, const vk::AttachmentReference& r)
  {
    return std::tie(l.attachment, l.layout) < std::tie(l.attachment, l.layout);
  }

  bool operator<(const vk::SubpassDependency& l, const vk::SubpassDependency& r)
  {
    return std::tie(l.srcSubpass, l.srcStageMask, l.srcAccessMask, l.dstSubpass, l.dstAccessMask, l.dstStageMask, l.dependencyFlags) <
           std::tie(r.srcSubpass, r.srcStageMask, r.srcAccessMask, r.dstSubpass, r.dstAccessMask, r.dstStageMask, r.dependencyFlags);
  }
}

namespace Vulkan
{
  bool SubpassKey::operator<(const SubpassKey& r) const
  {
    return std::tie(inputAttachmentReferences, outputColorAttachmentReferences) <
      std::tie(r.inputAttachmentReferences, r.outputColorAttachmentReferences);
  }

  RenderPassKey& RenderPassKey::SetSubpassesDescriptions(const std::vector<SubpassKey>& descs)
  {
    subpassesDescriptions = descs;
    return *this;
  }

  RenderPassKey& RenderPassKey::SetDependencies(const std::vector<vk::SubpassDependency>& deps)
  {
    dependencies = deps;
    return *this;
  }

  RenderPassKey& RenderPassKey::SetImageAttachments(const std::vector<ImageAttachment>& a)
  {
    imageAttachments = a;
    return *this;
  }



  RenderPassKey& RenderPassKey::SetBackbufferFormat(const vk::Format& f)
  {
    backbufferFormat = f;
    return *this;
  }

  bool RenderPassKey::operator<(const RenderPassKey& r) const
  {
    return std::tie(subpassesDescriptions, dependencies, imageAttachments, backbufferFormat) < std::tie(r.subpassesDescriptions, r.dependencies, r.imageAttachments, r.backbufferFormat);
  }

  RenderPassStorage::RenderPassStorage(Core& core)
    : core(core)
  {
  }

  vk::RenderPass RenderPassStorage::GetRenderPass(const RenderPassKey& key)
  {
    if (renderPasses.find(key) != renderPasses.end())
    {
      return renderPasses[key].get();
    }

    std::vector<vk::AttachmentDescription> attachmentDescriptions;
    for (const ImageAttachment& attachment : key.imageAttachments)
    {
      const auto desc = vk::AttachmentDescription()
        .setFormat(attachment.format)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setLoadOp(attachment.loadOp)
        .setStoreOp(attachment.storeOp)
        .setInitialLayout(attachment.initialLayout)
        .setFinalLayout(attachment.finalLayout);

      attachmentDescriptions.push_back(desc);
    }

    std::vector<vk::SubpassDescription> subpassesDescriptions;
    for (const SubpassKey& sk : key.subpassesDescriptions)
    {
      auto sd = vk::SubpassDescription()
        .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
        .setInputAttachmentCount(static_cast<uint32_t>(sk.inputAttachmentReferences.size()))
        .setPInputAttachments(sk.inputAttachmentReferences.data())
        .setColorAttachmentCount(static_cast<uint32_t>(sk.outputColorAttachmentReferences.size()))
        .setPColorAttachments(sk.outputColorAttachmentReferences.data());

      if (sk.depthStencilAttachmentReference.has_value())
        sd.setPDepthStencilAttachment(&sk.depthStencilAttachmentReference.value());

      subpassesDescriptions.push_back(std::move(sd));
    }

    const auto rpCreateInfo = vk::RenderPassCreateInfo()
      .setAttachmentCount(static_cast<uint32_t>(attachmentDescriptions.size()))
      .setPAttachments(attachmentDescriptions.data())
      .setSubpassCount(static_cast<uint32_t>(subpassesDescriptions.size()))
      .setPSubpasses(subpassesDescriptions.data())
      .setDependencyCount(static_cast<uint32_t>(key.dependencies.size()))
      .setPDependencies(key.dependencies.data());

    vk::UniqueRenderPass rpUnique = core.GetLogicalDevice().createRenderPassUnique(rpCreateInfo);
    vk::RenderPass rp = rpUnique.get();
    renderPasses[key] = std::move(rpUnique);

    return rp;
  }
}