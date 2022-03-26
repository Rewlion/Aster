#pragma once

#include "indices.h"
#include "resources.h"
#include "swapchain.h"

#include <engine/types.h>
#include <engine/utils/fixed_pool.hpp>

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
    friend class DescriptorsSetManager;

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

      inline uint8_t getBackbufferId() const { return m_Swapchain.getBackbufferId(); }

      inline vk::Extent2D getSurfaceExtent() const { return m_Swapchain.getSurfaceExtent(); };

      vk::CommandBuffer allocateGraphicsCmdBuffer();

      vk::Format getTextureFormat(const TextureHandler handler);

      vk::ImageView getImageView(const TextureHandler handler);

      vk::Image getImage(const TextureHandler handler);

      vk::Extent3D getImageDim(const TextureHandler handler);

      void submitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf);

      void transitSurfaceImageForPresent();
      void presentSurfaceImage();

      BufferHandler allocateBuffer(const BufferAllocationDescription& allocDesc);

      void copyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);

      vk::Buffer getBuffer(const BufferHandler buffer);

      TextureHandler allocateTexture(const TextureAllocationDescription& allocDesc);

      void copyToTextureSync(const void* src, const size_t size, const TextureHandler texture);

      SamplerHandler allocateSampler(const SamplerAllocationDescription& allocDesc);

      vk::Sampler getSampler(const SamplerHandler sampler);

      vk::ImageLayout getImageLayout(const TextureHandler handler);

      void setImageLayout(const TextureHandler handler, const vk::ImageLayout layout);

    private:
      Buffer allocateBufferInternal(const BufferAllocationDescription& allocDesc, const uint32_t memoryIndex);
      Buffer allocateStagingBuffer(const size_t size);
      Buffer allocateStagingBuffer(const void* src, const size_t size);

      vk::CommandBuffer allocateCmdBuffer(vk::CommandPool pool);
      vk::CommandBuffer allocateTransferCmdBuffer();

    private:
      vk::UniqueDevice m_Device;

      Swapchain m_Swapchain;

      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;

      vk::Queue m_GraphicsQueue;
      vk::Queue m_TransferQueue;

      vk::UniqueCommandPool m_GraphicsCmdPool;
      vk::UniqueCommandPool m_TransferCmdPool;

      Utils::FixedPool<Buffer, 1024> m_AllocatedBuffers;
      Utils::FixedPool<Texture, 1024> m_AllocatedTextures;
      Utils::FixedPool<Sampler, 64> m_AllocatedSamplers;
  };
}
