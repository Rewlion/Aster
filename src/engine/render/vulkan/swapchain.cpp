#include "swapchain.h"

#include <engine/assert.h>

namespace gapi::vulkan
{
  Swapchain::Swapchain(const CreateInfo& ci)
  {
    m_Device = ci.device;
    m_PresentQueue = ci.presentQueue;
    m_SurfaceFormat = GetSuitableSurfaceFormat(ci.surfaceFormats);
    m_SurfaceExtent = GetSwapchainExtent(ci.surfaceCapabilities, ci.swapchainImageExtent);
    m_Swapchain = CreateSwapchain(ci, m_SurfaceFormat, m_SurfaceExtent);
    SetSwapchainResources();
  }

  vk::UniqueSwapchainKHR Swapchain::CreateSwapchain(
    const CreateInfo& ci,
    const vk::SurfaceFormatKHR& surfaceFormat,
    const vk::Extent2D& surfaceExtent) const
  {
    const auto swapchainPresentMode = GetSwapchainPresentMode(ci.surfacePresentModes, vk::PresentModeKHR::eFifo);
    const auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
          .setSurface(ci.surface)
          .setMinImageCount(SWAPCHAIN_IMAGES_COUNT)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(surfaceExtent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(ci.surfaceCapabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(swapchainPresentMode)
          .setClipped(true)
          .setOldSwapchain(nullptr);

    return ci.device.createSwapchainKHRUnique(swapchainCreateInfo);
  }

  vk::SurfaceFormatKHR Swapchain::GetSuitableSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
  {
    return { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
  }

  vk::PresentModeKHR Swapchain::GetSwapchainPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes,
        const vk::PresentModeKHR& preferredMode) const
  {
    bool hasFifo = false;

    for (const auto& mode : availablePresentModes)
    {
      if (mode == preferredMode)
        return preferredMode;

      if (mode == vk::PresentModeKHR::eFifo)
        hasFifo = true;
    }

    ASSERT(hasFifo);

    return vk::PresentModeKHR::eFifo;
  }

  vk::Extent2D Swapchain::GetSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& windowSize) const
  {
    if (capabilities.currentExtent.width != uint32_t(-1))
    {
      return capabilities.currentExtent;
    }
    else
    {
      vk::Extent2D actualExtent = windowSize;
      actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
      return actualExtent;
    }
  }

  void Swapchain::SetSwapchainResources()
  {
    std::vector<vk::Image> images = m_Device.getSwapchainImagesKHR(*m_Swapchain);
    for (size_t i = 0; i < images.size(); ++i)
    {
      m_SwapchainResources.images[i] = images[i];

      const auto& compMap = vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
      vk::ImageSubresourceRange subresourceRange;
      subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = 1;
      subresourceRange.baseArrayLayer = 0;
      subresourceRange.layerCount = 1;

      const auto imgViewCi = vk::ImageViewCreateInfo()
        .setImage(images[i])
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(m_SurfaceFormat.format)
        .setComponents(compMap)
        .setSubresourceRange(subresourceRange);
      m_SwapchainResources.views[i] = m_Device.createImageViewUnique(imgViewCi);

      m_SwapchainResources.imageAcquiredFence = m_Device.createFenceUnique(vk::FenceCreateInfo{});

      const auto semaphoreTypeCi = vk::SemaphoreTypeCreateInfo()
        .setInitialValue(0)
        .setSemaphoreType(vk::SemaphoreType::eBinary);

      const auto semaphoreCi = vk::SemaphoreCreateInfo()
        .setPNext(&semaphoreTypeCi);

      m_SwapchainResources.waitForRenderFinishedSemaphores[i] = m_Device.createSemaphoreUnique(semaphoreCi);
    }
  }

  void Swapchain::BeginFrame()
  {
    frameId = m_Device.acquireNextImageKHR(*m_Swapchain, -1, {}, *m_SwapchainResources.imageAcquiredFence).value;
    const vk::Result r = m_Device.waitForFences(1, &m_SwapchainResources.imageAcquiredFence.get(), true, -1);
    ASSERT(r == vk::Result::eSuccess);
  }

  void Swapchain::EndFrame()
  {
    vk::SwapchainKHR swapchains[] {m_Swapchain.get()};
    const auto presentInfo = vk::PresentInfoKHR()
      .setSwapchainCount(1)
      .setPSwapchains(swapchains)
      .setPImageIndices(&frameId)
      .setPResults(nullptr)
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(GetWaitForRenderFinishedSemaphore());

    const vk::Result r = m_PresentQueue.presentKHR(presentInfo);
    ASSERT(r == vk::Result::eSuccess);
  }
}
