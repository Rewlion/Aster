#include "swapchain.h"
#include "result.h"


#include <engine/assert.h>

namespace gapi::vulkan
{
  Swapchain::Swapchain(const CreateInfo& ci)
  {
    m_Device = ci.device;
    m_PresentQueue = ci.presentQueue;
    m_SurfaceFormat = getSuitableSurfaceFormat(ci.surfaceFormats);
    m_SurfaceExtent = getSwapchainExtent(ci.surfaceCapabilities, ci.swapchainImageExtent);
    m_Swapchain = createSwapchain(ci, m_SurfaceFormat, m_SurfaceExtent);
    setSwapchainResources(*ci.initCmdBuf);
  }

  Swapchain& Swapchain::operator=(Swapchain&& rvl)
  {
    m_Device = rvl.m_Device;
    m_PresentQueue = rvl.m_PresentQueue;
    m_SurfaceFormat = rvl.m_SurfaceFormat;
    m_SurfaceExtent = rvl.m_SurfaceExtent;
    m_Swapchain = std::move(rvl.m_Swapchain);
    m_SwapchainResources = std::move(rvl.m_SwapchainResources);
    frameId = rvl.frameId;
    return *this;
  }

  vk::UniqueSwapchainKHR Swapchain::createSwapchain(
    const CreateInfo& ci,
    const vk::SurfaceFormatKHR& surfaceFormat,
    const vk::Extent2D& surfaceExtent) const
  {
    const auto swapchainPresentMode = getSwapchainPresentMode(ci.surfacePresentModes, vk::PresentModeKHR::eFifo);
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

    auto swp = ci.device.createSwapchainKHRUnique(swapchainCreateInfo);
    VK_CHECK_RES(swp);

    return std::move(swp.value);
  }

  vk::SurfaceFormatKHR Swapchain::getSuitableSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
  {
    return { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
  }

  vk::PresentModeKHR Swapchain::getSwapchainPresentMode(
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

  vk::Extent2D Swapchain::getSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& windowSize) const
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

  void Swapchain::setSwapchainResources(vk::CommandBuffer& initCmdBuf)
  {
    auto imgRes = m_Device.getSwapchainImagesKHR(*m_Swapchain);
    VK_CHECK_RES(imgRes);
    const std::vector<vk::Image>& images = imgRes.value;
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

      auto view = m_Device.createImageViewUnique(imgViewCi);
      VK_CHECK_RES(view);
      m_SwapchainResources.views[i] = std::move(view.value);

      vk::ImageMemoryBarrier layoutBarrier;
      layoutBarrier.image = images[i];
      layoutBarrier.oldLayout = vk::ImageLayout::eUndefined;
      layoutBarrier.newLayout = vk::ImageLayout::ePresentSrcKHR;
      layoutBarrier.subresourceRange = subresourceRange;
      layoutBarrier.srcAccessMask = {};
      layoutBarrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;

      initCmdBuf.pipelineBarrier(
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::DependencyFlags{},
        0, nullptr,
        0, nullptr,
        1, &layoutBarrier
      );
    }
  }

  void Swapchain::acquireSurfaceImage(const vk::Semaphore waitSemaphore)
  {
    frameId = m_Device.acquireNextImageKHR(*m_Swapchain, -1, waitSemaphore, {}).value;
  }

  void Swapchain::present(vk::Semaphore wait_semaphore)
  {
    vk::SwapchainKHR swapchains[] {m_Swapchain.get()};
    const auto presentInfo = vk::PresentInfoKHR()
      .setSwapchainCount(1)
      .setPSwapchains(swapchains)
      .setPImageIndices(&frameId)
      .setPResults(nullptr)
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(&wait_semaphore);

    VK_CHECK(m_PresentQueue.presentKHR(presentInfo));
  }
}
