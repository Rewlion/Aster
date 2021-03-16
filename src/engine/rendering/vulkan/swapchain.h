#pragma once

#include "image.h"

#include <limits>
#include <algorithm>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#undef VK_USE_PLATFORM_WIN32_KHR

#undef max
#undef min

namespace Vulkan
{
  struct SurfaceDetails
  {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;

    SurfaceDetails& SetCapabilities(const vk::SurfaceCapabilitiesKHR& capabilities)
    {
      this->capabilities = capabilities;
      return *this;
    }

    SurfaceDetails& SetFormats(const std::vector<vk::SurfaceFormatKHR>& formats)
    {
      this->formats = formats;
      return *this;
    }

    SurfaceDetails& SetPresentModes(const std::vector<vk::PresentModeKHR>& presentModes)
    {
      this->presentModes = presentModes;
      return *this;
    }
  };

  class Swapchain
  {
  public:
    Swapchain(vk::SurfaceKHR surface, const vk::Extent2D& wndSize, vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, uint32_t presentFamilyIndex, uint32_t graphicsFamilyIndex, vk::PresentModeKHR preferredMode, uint32_t imagesCount, vk::Queue presentQueue)
    {
      this->instance = instance;

      this->physicalDevice = physicalDevice;

      this->logicalDevice = logicalDevice;

      this->surface = surface;

      this->surfaceDetails = SurfaceDetails()
        .SetCapabilities(physicalDevice.getSurfaceCapabilitiesKHR(surface))
        .SetFormats(physicalDevice.getSurfaceFormatsKHR(surface))
        .SetPresentModes(physicalDevice.getSurfacePresentModesKHR(surface));

      this->surfaceFormat = FindSwapchainSurfaceFormat(surfaceDetails.formats);

      this->presentMode = FindSwapchainPresentMode(surfaceDetails.presentModes, preferredMode);

      this->extent = FindSwapchainExtent(surfaceDetails.capabilities, wndSize);

      this->imgsCount = GetImagesCount(surfaceDetails.capabilities, imagesCount);

      {
        auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
          .setSurface(surface)
          .setMinImageCount(imagesCount)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(extent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(surfaceDetails.capabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(presentMode)
          .setClipped(true)
          .setOldSwapchain(nullptr);

        uint32_t familyIndices[] = { graphicsFamilyIndex, presentFamilyIndex };

        if (graphicsFamilyIndex != presentFamilyIndex)
        {
          swapchainCreateInfo
            .setImageSharingMode(vk::SharingMode::eConcurrent)
            .setQueueFamilyIndexCount(2)
            .setPQueueFamilyIndices(familyIndices);
        }
        else
        {
          swapchainCreateInfo
            .setImageSharingMode(vk::SharingMode::eExclusive);
        }

        this->swapchain = logicalDevice.createSwapchainKHRUnique(swapchainCreateInfo);
      }

      std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(*swapchain);
      for (auto& image : images)
      {
        Image img{ logicalDevice, image, surfaceFormat.format, extent };
        swapchainImages.push_back(std::move(img));
      }

      this->presentQueue = presentQueue;
    }

    ImageView AcquireNextImage(vk::Semaphore semaphore)
    {
      acquiredImageIndex = logicalDevice.acquireNextImageKHR(*swapchain, -1, semaphore, nullptr).value;

      return swapchainImages[acquiredImageIndex].GetView();
    }

    void PresentImage(vk::Semaphore waitSemaphore)
    {
      vk::SwapchainKHR swapchains[] = { *swapchain };
      vk::Semaphore waitSemaphores[] = { waitSemaphore };

      const auto presentInfo = vk::PresentInfoKHR()
        .setSwapchainCount(1)
        .setPSwapchains(swapchains)
        .setPImageIndices(&acquiredImageIndex)
        .setPResults(nullptr)
        .setPWaitSemaphores(waitSemaphores)
        .setWaitSemaphoreCount(1);

      presentQueue.presentKHR(presentInfo);
    }

    vk::Format GetImageFormat() const
    {
      return surfaceFormat.format;
    }

    vk::Extent2D GetSurfaceSize() const
    {
      return extent;
    }

    inline uint32_t GetImagesCount() const
    {
      return imgsCount;
    }

  private:
    vk::SurfaceFormatKHR FindSwapchainSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
    {
      vk::SurfaceFormatKHR bestFormat = { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
      if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
      {
        return bestFormat;
      }

      for (const auto& availableFormat : availableFormats)
      {
        if (availableFormat.format == bestFormat.format && availableFormat.colorSpace == bestFormat.colorSpace)
          return availableFormat;
      }

      throw std::runtime_error("No suitable format found");

      return bestFormat;
    }

    vk::PresentModeKHR FindSwapchainPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes, vk::PresentModeKHR preferredMode) const
    {
      for (const auto& availablePresentMode : availablePresentModes)
      {
        if (availablePresentMode == preferredMode)
          return availablePresentMode;
      }

      return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D FindSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D windowSize) const
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

    uint32_t GetImagesCount(const vk::SurfaceCapabilitiesKHR& capabilities, const uint32_t imagesCount) const
    {
      uint32_t count = std::max(capabilities.minImageCount, imagesCount);

      if (capabilities.maxImageCount > 0 && imagesCount > capabilities.maxImageCount)
        count = capabilities.maxImageCount;

      return count;
    }

  private:
    vk::Instance instance;
    vk::PhysicalDevice physicalDevice;
    vk::Device logicalDevice;
    vk::SurfaceKHR surface;
    SurfaceDetails surfaceDetails;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    vk::Extent2D extent;
    uint32_t imgsCount;
    vk::UniqueSwapchainKHR swapchain;
    std::vector<Image> swapchainImages;
    uint32_t acquiredImageIndex;
    vk::Queue presentQueue;
  };
}