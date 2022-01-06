#pragma once

#include "indices.h"

#include <engine/types.h>

#include <vulkan/vulkan.hpp>

#include <EASTL/vector.h>
#include <vector>

namespace gapi::vulkan
{
  constexpr int SWAPCHAIN_IMAGES_COUNT = 2;

  class Image
  {
    friend class Device;
    public:
      enum class Type: uint8_t
      {
        None = 0,
        Swapchain = 1
      };

    public:
      inline ~Image()
      {
        if (m_ImageOwner)
          vkDestroyImage(m_Device, m_Image, NULL);
      }

    private:
      vk::Image m_Image;
      vk::Device m_Device;

      bool m_ImageOwner = false;
      Type m_Type = Type::None;

  };

  class Device
  {
    public:
      struct CreateInfo
      {
        vk::Instance instance;
        vk::SurfaceKHR surface;
        vk::UniqueDevice device;
        QueueIndices queueIndices;
        MemoryIndices memoryIndices;

        vk::SurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> surfaceFormats;
        std::vector<vk::PresentModeKHR> surfacePresentModes;

        vk::Extent2D swapchainImageExtent = vk::Extent2D{1920, 1024};
      };

    public:
      Device() = default;
      Device(CreateInfo&&);

    private:
      vk::UniqueSwapchainKHR createSwapchain(const CreateInfo& ci) const;

      vk::SurfaceFormatKHR getSuitableSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR>& availableFormats) const;

      vk::PresentModeKHR getSwapchainPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes,
        const vk::PresentModeKHR& preferredMode) const;

      vk::Extent2D getSwapchainExtent(
        const vk::SurfaceCapabilitiesKHR& capabilities,
        const vk::Extent2D& windowSize) const;

      void setSwapchainResources();

    private:
      vk::UniqueDevice m_Device;

      vk::UniqueSwapchainKHR m_Swapchain;
      struct SwapchainResources
      {
        Image images[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueFence imageAcquiredFences[SWAPCHAIN_IMAGES_COUNT];
      };
      SwapchainResources m_SwapchainResources;

      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;

      vk::Queue m_GraphicsQueue;
      vk::Queue m_PresentQueue;
      vk::Queue m_TransferQueue;

      vk::UniqueCommandPool m_GraphicsCmdPool;
  };
}
