#pragma once

#include "resources.h"

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  class Swapchain
  {
    public:
      struct CreateInfo
      {
        vk::SurfaceKHR surface;
        vk::Device device;
        vk::Queue presentQueue;

        vk::SurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> surfaceFormats;
        std::vector<vk::PresentModeKHR> surfacePresentModes;

        vk::Extent2D swapchainImageExtent = vk::Extent2D{1920, 1024};
      };

      Swapchain() = default;
      Swapchain(const CreateInfo& ci);

      void Present();

      inline uint8_t GetSurfaceRtId() const
      {
        return frameId;
      }

      inline vk::Extent2D GetSurfaceExtent() const
      {
        return m_SurfaceExtent;
      };

      inline vk::Format GetSurfaceFormat() const
      {
        return m_SurfaceFormat.format;
      }

      inline vk::ImageView GetSurfaceImageView() const
      {
        return m_SwapchainResources.views[frameId].get();
      }

      inline vk::Image GetSurfaceImage() const
      {
        return m_SwapchainResources.images[frameId];
      }

      inline const vk::Semaphore* GetWaitForRenderFinishedSemaphore() const
      {
        return &m_SwapchainResources.waitForRenderFinishedSemaphores[frameId].get();
      }

    private:
      
      void AcquireSurfaceImage();

      vk::UniqueSwapchainKHR CreateSwapchain(
        const CreateInfo& ci,
        const vk::SurfaceFormatKHR& surfaceFormat,
        const vk::Extent2D& surfaceExtent) const;

      vk::SurfaceFormatKHR GetSuitableSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;

      vk::PresentModeKHR GetSwapchainPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes,
        const vk::PresentModeKHR& preferredMode) const;

      vk::Extent2D GetSwapchainExtent(
        const vk::SurfaceCapabilitiesKHR& capabilities,
        const vk::Extent2D& windowSize) const;

      void SetSwapchainResources();

    private:
      vk::Device m_Device;
      vk::Queue m_PresentQueue;

      vk::SurfaceFormatKHR m_SurfaceFormat;
      vk::Extent2D m_SurfaceExtent;
      vk::UniqueSwapchainKHR m_Swapchain;
      struct SwapchainResources
      {
        vk::Image images[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueImageView views[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueSemaphore waitForRenderFinishedSemaphores[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueFence imageAcquiredFence;
      };
      SwapchainResources m_SwapchainResources;
      uint32_t frameId = 0;
  };
}