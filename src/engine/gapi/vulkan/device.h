#pragma once

#include "indices.h"
#include "resources.h"
#include "swapchain.h"

#include <engine/types.h>
#include <engine/utils/fixed_pool.hpp>

#include <EASTL/vector.h>
#include <vector>

namespace gapi
{
  class CmdEncoder;
}

namespace gapi::vulkan
{
  class FrameGarbageCollector;

  class Device
  {
    friend class RenderPassStorage;
    friend class CompileContext;
    friend class ShadersStorage;
    friend class PipelinesStorage;
    friend class PipelineLayoutsStorage;
    friend class DescriptorsSetManager;

    public:
      enum class CapabilitiesBits
      {
        DebugMarks = 0,
        Count
      };
      using Capabilities = Utils::BitCapacity<(size_t)CapabilitiesBits::Count>;

      struct CreateInfo
      {
        vk::PhysicalDevice physicalDevice;
        vk::Instance instance;
        vk::SurfaceKHR surface;
        vk::UniqueDevice device;
        vk::PhysicalDeviceProperties deviceProperties;
        QueueIndices queueIndices;
        MemoryIndices memoryIndices;

        vk::SurfaceCapabilitiesKHR surfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> surfaceFormats;
        std::vector<vk::PresentModeKHR> surfacePresentModes;

        vk::Extent2D swapchainImageExtent = vk::Extent2D{1920, 1024};

        Capabilities capabilities;
      };

    public:
      Device() = default;
      Device(CreateInfo&&, FrameGarbageCollector*);
      ~Device();

      const Device& operator=(const Device& rvl) = delete;

      auto checkCapability(const CapabilitiesBits) const -> bool;

      inline uint8_t getBackbufferId() const { return m_Swapchain.getBackbufferId(); }
      inline TextureHandle getBackbuffer() const
      {
        TextureHandlerInternal h;
        h.as.typed.id = getBackbufferId();
        h.as.typed.type = (uint64_t)TextureType::SurfaceRT;

        return h;
      }

      inline vk::Extent2D getSurfaceExtent() const { return m_Swapchain.getSurfaceExtent(); };

      vk::CommandBuffer allocateGraphicsCmdBuffer();

      vk::Format getTextureFormat(const TextureHandle handler);

      auto getTextureExtent(const TextureHandle) -> uint3;

      vk::ImageView getImageView(const TextureHandle handler, const bool srv = false, const size_t mip = 0);

      auto getImageViewType(const TextureHandle) const -> vk::ImageViewType;
      
      vk::Image getImage(const TextureHandle handler);

      vk::Extent3D getImageDim(const TextureHandle handler);

      void submitGraphicsCmd(vk::CommandBuffer, const vk::Fence signal_fence);

      void presentSurfaceImage();

      BufferHandler allocateBuffer(const size_t size, const int usage, const char* name);
      void freeBuffer(const BufferHandler buffer);

      void* mapBuffer(const BufferHandler buffer, const size_t offset, const size_t size, const int flags = 0);
      void unmapBuffer(const BufferHandler buffer);

      void copyBuffersSync(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size);

      Buffer& getBuffer(const BufferHandler buffer);
      const Buffer& getBuffer(const BufferHandler buffer) const;

      TextureHandle allocateTexture(const TextureAllocationDescription& allocDesc);
      void freeTexture(const TextureHandle texture);

      SamplerHandler allocateSampler(const SamplerAllocationDescription& allocDesc);
      void freeSampler(const SamplerHandler sampler);

      vk::Sampler getSampler(const SamplerHandler sampler);

      inline vk::Device& getDevice()
      {
        return m_Device.get();
      }

      vk::UniqueSemaphore createSemaphore();

      void acquireBackbuffer(vk::Fence signal_fence);

      void discardBuffer(Buffer& buffer);
      Buffer* getAllocatedBuffer(const BufferHandler handler);
      Texture& getAllocatedTexture(const TextureHandle texture);

      Buffer allocateBufferInternal(const size_t size, const int usage);
      Buffer allocateStagingBuffer(const void* src, const size_t size);

      vk::UniqueCommandPool allocateCmdPool();

      VulkanFence* allocateFence();

      auto allocateCmdPromise() -> VulkanCmdPromise*;

      void prepareSrvStubs(gapi::CmdEncoder&);

      void setDbgUtilsObjName(const char* name, const uint64_t obj, const vk::ObjectType type);

    private:
      void copyBuffersSync(const vk::Buffer src, const size_t srcOffset, const vk::Buffer dst, const size_t dstOffset, const size_t size);
      void writeToStagingBuffer(const Buffer& buffer, const void* src, const size_t offset, const size_t size);
      void* mapBuffer(const Buffer& buffer, const size_t offset, const size_t size);
      void unmapBuffer(const Buffer& buffer);


      vk::CommandBuffer allocateCmdBuffer(vk::CommandPool pool);
      vk::CommandBuffer allocateTransferCmdBuffer();

    private:
      Capabilities m_Capabilities;

      FrameGarbageCollector* m_FrameGc;
      vk::UniqueDevice m_Device;
      vk::PhysicalDeviceProperties m_DeviceProperties;

      Swapchain m_Swapchain;
      eastl::vector<vk::Semaphore> m_SemaphoresToWaitBeforePresent;
      vk::Semaphore m_BackbufferAcquireSemaphore;

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
