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

      inline uint8_t getSurfaceRtId() const { return m_Swapchain.GetSurfaceRtId(); }

      inline vk::Extent2D getSurfaceExtent() const { return m_Swapchain.GetSurfaceExtent(); };

      vk::CommandBuffer AllocateGraphicsCmdBuffer();

      vk::Format getTextureFormat(const TextureHandler handler);

      vk::ImageView getImageView(const TextureHandler handler);

      vk::Image GetImage(const TextureHandler handler);

      vk::Extent3D GetImageDim(const TextureHandler handler);

      void SubmitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf);

      void TransitSurfaceImageForPresent();
      void PresentSurfaceImage();

      BufferHandler AllocateBuffer(const BufferAllocationDescription& allocDesc);

      void CopyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);

      vk::Buffer GetBuffer(const BufferHandler buffer);

      TextureHandler AllocateTexture(const TextureAllocationDescription& allocDesc);

      void CopyToTextureSync(const void* src, const size_t size, const TextureHandler texture);

      SamplerHandler AllocateSampler(const SamplerAllocationDescription& allocDesc);

      vk::Sampler GetSampler(const SamplerHandler sampler);

      vk::ImageLayout GetImageLayout(const TextureHandler handler);

      void SetImageLayout(const TextureHandler handler, const vk::ImageLayout layout);

    private:
      Buffer AllocateBufferInternal(const BufferAllocationDescription& allocDesc, const uint32_t memoryIndex);
      Buffer AllocateStagingBuffer(const size_t size);
      Buffer AllocateStagingBuffer(const void* src, const size_t size);

      vk::CommandBuffer AllocateCmdBuffer(vk::CommandPool pool);
      vk::CommandBuffer AllocateTransferCmdBuffer();

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
