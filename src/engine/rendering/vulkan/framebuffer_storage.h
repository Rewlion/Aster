#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <vector>
#include <tuple>
#include <map>

namespace Vulkan
{
  struct FramebufferKey
  {
    vk::RenderPass renderPass;
    std::vector<vk::ImageView> attachments;
    uint32_t width;
    uint32_t height;
    uint32_t layers;

    bool operator< (const FramebufferKey& r) const;

    FramebufferKey& SetRenderPass(vk::RenderPass renderPass);

    FramebufferKey& SetAttachments(const std::vector<vk::ImageView>& attachments);

    FramebufferKey& SetWidth(uint32_t width);

    FramebufferKey& SetHeight(uint32_t height);

    FramebufferKey& SetLayers(uint32_t layers);
  };

  class FramebufferStorage
  {
  public:
    FramebufferStorage(vk::Device logicalDevice);

    vk::Framebuffer GetFramebuffer(const FramebufferKey& key);

    vk::Framebuffer AddNewFramebuffer(const FramebufferKey& key);

  private:
    vk::Device logicalDevice;
    std::map<FramebufferKey, vk::UniqueFramebuffer> storage;
  };
}
