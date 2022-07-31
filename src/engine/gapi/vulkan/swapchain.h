#pragma once

#include "vulkan.h"
#include "resources.h"


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
        vk::CommandBuffer* initCmdBuf;
      };

      Swapchain() = default;
      Swapchain(const CreateInfo& ci);

      void present();

      inline uint8_t getBackbufferId() const
      {
        return frameId;
      }

      inline vk::Extent2D getSurfaceExtent() const
      {
        return m_SurfaceExtent;
      };

      inline vk::Format getSurfaceFormat() const
      {
        return m_SurfaceFormat.format;
      }

      inline vk::ImageView getSurfaceImageView() const
      {
        return m_SwapchainResources.views[frameId].get();
      }

      inline vk::Image getSurfaceImage() const
      {
        return m_SwapchainResources.images[frameId];
      }

      inline vk::Extent2D getSurfaceDim() const
      {
        return m_SurfaceExtent;
      }

      void acquireSurfaceImage(const vk::Semaphore waitSemaphore);

    private:

      vk::UniqueSwapchainKHR createSwapchain(
        const CreateInfo& ci,
        const vk::SurfaceFormatKHR& surfaceFormat,
        const vk::Extent2D& surfaceExtent) const;

      vk::SurfaceFormatKHR getSuitableSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;

      vk::PresentModeKHR getSwapchainPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes,
        const vk::PresentModeKHR& preferredMode) const;

      vk::Extent2D getSwapchainExtent(
        const vk::SurfaceCapabilitiesKHR& capabilities,
        const vk::Extent2D& windowSize) const;

      void setSwapchainResources(vk::CommandBuffer& initCmdBuf);

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
      };
      SwapchainResources m_SwapchainResources;
      uint32_t frameId = 0;
  };
}