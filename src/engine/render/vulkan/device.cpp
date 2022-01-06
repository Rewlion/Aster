#include "device.h"

#include <engine/assert.h>

namespace gapi::vulkan
{
  Device::Device(CreateInfo&& ci)
    : m_Device(std::move(ci.device))
    , m_QueueIndices(ci.queueIndices)
    , m_MemoryIndices(ci.memoryIndices)
  {
    vk::Instance& instance = ci.instance;

    m_Swapchain = createSwapchain(ci);
    setSwapchainResources();

    m_GraphicsQueue = m_Device->getQueue(ci.queueIndices.graphics, 0);
    m_PresentQueue = m_Device->getQueue(ci.queueIndices.present, 0);
    m_TransferQueue = m_Device->getQueue(ci.queueIndices.transfer, 0);

    const auto cmdPoolCreateInfo = vk::CommandPoolCreateInfo()
      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
      .setQueueFamilyIndex(ci.queueIndices.graphics);

    m_GraphicsCmdPool = m_Device->createCommandPoolUnique(cmdPoolCreateInfo);
  }

  vk::UniqueSwapchainKHR Device::createSwapchain(const CreateInfo& ci) const
  {
    const auto swapchainImageFormat = getSuitableSurfaceFormat(ci.surfaceFormats);
    const auto swapchainImageExtent = getSwapchainExtent(ci.surfaceCapabilities, ci.swapchainImageExtent);
    const auto swapchainPresentMode = getSwapchainPresentMode(ci.surfacePresentModes, vk::PresentModeKHR::eFifo);

    const auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
          .setSurface(ci.surface)
          .setMinImageCount(SWAPCHAIN_IMAGES_COUNT)
          .setImageFormat(swapchainImageFormat.format)
          .setImageColorSpace(swapchainImageFormat.colorSpace)
          .setImageExtent(swapchainImageExtent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(ci.surfaceCapabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(swapchainPresentMode)
          .setClipped(true)
          .setOldSwapchain(nullptr);

    return m_Device->createSwapchainKHRUnique(swapchainCreateInfo);
  }

  vk::SurfaceFormatKHR Device::getSuitableSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
  {
    return { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
  }

  vk::PresentModeKHR Device::getSwapchainPresentMode(
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

  vk::Extent2D Device::getSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& windowSize) const
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

  void Device::setSwapchainResources()
  {
    std::vector<vk::Image> images = m_Device->getSwapchainImagesKHR(*m_Swapchain);
    for (size_t i = 0; i < images.size(); ++i)
    {
      m_SwapchainResources.images[i].m_Image = images[i];
      m_SwapchainResources.images[i].m_Type = Image::Type::Swapchain;

      m_SwapchainResources.imageAcquiredFences[i] = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    }
  }
}