#include "framebuffer_storage.h"

namespace Vulkan
{
  bool FramebufferKey::operator< (const FramebufferKey& r) const
  {
    return std::tie(renderPass, attachments, width, height, layers) <
      std::tie(r.renderPass, r.attachments, r.width, r.height, r.layers);
  }

  FramebufferKey& FramebufferKey::SetRenderPass(vk::RenderPass renderPass)
  {
    this->renderPass = renderPass;
    return *this;
  }

  FramebufferKey& FramebufferKey::SetAttachments(const std::vector<vk::ImageView>& attachments)
  {
    this->attachments = attachments;
    return *this;
  }

  FramebufferKey& FramebufferKey::SetWidth(uint32_t width)
  {
    this->width = width;
    return *this;
  }

  FramebufferKey& FramebufferKey::SetHeight(uint32_t height)
  {
    this->height = height;
    return *this;
  }

  FramebufferKey& FramebufferKey::SetLayers(uint32_t layers)
  {
    this->layers = layers;
    return *this;
  }

  FramebufferStorage::FramebufferStorage(vk::Device logicalDevice)
    : logicalDevice(logicalDevice)
  {}

  vk::Framebuffer FramebufferStorage::GetFramebuffer(const FramebufferKey& key)
  {
    const auto it = storage.find(key);

    if (it == storage.end())
      return AddNewFramebuffer(key);

    return storage[key].get();
  }

  vk::Framebuffer FramebufferStorage::AddNewFramebuffer(const FramebufferKey& key)
  {
    const auto framebufferCreateInfo = vk::FramebufferCreateInfo()
      .setRenderPass(key.renderPass)
      .setAttachmentCount(key.attachments.size())
      .setPAttachments(key.attachments.data())
      .setWidth(key.width)
      .setHeight(key.height)
      .setLayers(key.layers);

    vk::UniqueFramebuffer ufb = logicalDevice.createFramebufferUnique(framebufferCreateInfo);
    const vk::Framebuffer fb = ufb.get();

    storage[key] = std::move(ufb);

    return fb;
  }
}
