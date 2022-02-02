#pragma once

#include "indices.h"
#include "resources.h"
#include "swapchain.h"

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
    friend class PipelinesStorage;
    friend class PipelineLayoutsStorage;

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

      inline uint8_t getSurfaceRtId() const { return m_Swapchain.GetSurfaceRtId(); }

      inline vk::Extent2D getSurfaceExtent() const { return m_Swapchain.GetSurfaceExtent(); };

      vk::CommandBuffer allocateGraphicsCmdBuffer();

      vk::Format getTextureFormat(const TextureHandler handler);

      vk::ImageView getImageView(const TextureHandler handler);

      vk::Extent3D GetImageDim(const TextureHandler handler);

      void SubmitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf);

      void TransitSurfaceImageForPresent();
      void PresentSurfaceImage();

    private:
      vk::UniqueDevice m_Device;

      Swapchain m_Swapchain;

      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;

      vk::Queue m_GraphicsQueue;
      vk::Queue m_TransferQueue;

      vk::UniqueCommandPool m_GraphicsCmdPool;
  };
}
