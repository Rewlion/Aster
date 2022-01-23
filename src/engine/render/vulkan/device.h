#pragma once

#include "indices.h"
#include "resources.h"

#include <engine/types.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>
#include <vector>

namespace gapi::vulkan
{
  class Device
  {
    friend class RenderPassStorage;
    friend class CompileContext;
    friend class ShadersStorage;

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

      inline uint8_t getSurfaceRtId() const { return frameId; }

      inline vk::Extent2D getSurfaceExtent() const { return m_SurfaceExtent; };

      vk::CommandBuffer allocateGraphicsCmdBuffer();

      vk::Format getTextureFormat(const TextureHandler handler);

      vk::ImageView getImageView(const TextureHandler handler);

      void beginFrame();
      void endFrame();

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

      void setSwapchainResources();

    private:
      vk::UniqueDevice m_Device;

      vk::SurfaceFormatKHR m_SurfaceFormat;
      vk::Extent2D m_SurfaceExtent;
      vk::UniqueSwapchainKHR m_Swapchain;
      struct SwapchainResources
      {
        vk::Image images[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueImageView views[SWAPCHAIN_IMAGES_COUNT];
        vk::UniqueFence imageAcquiredFence;
        vk::UniqueSemaphore renderingFinishedSemaphores[SWAPCHAIN_IMAGES_COUNT];
      };
      SwapchainResources m_SwapchainResources;
      uint32_t frameId = 0;

      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;

      vk::Queue m_GraphicsQueue;
      vk::Queue m_PresentQueue;
      vk::Queue m_TransferQueue;

      vk::UniqueCommandPool m_GraphicsCmdPool;
  };
}
