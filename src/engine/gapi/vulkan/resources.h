#pragma once

#include "vulkan.h"
#include "result.h"

#include <engine/assert.h>
#include <engine/gapi/resources.h>
#include <engine/platform/memory.h>

#include <stdint.h>


namespace gapi::vulkan
{
  constexpr int SWAPCHAIN_IMAGES_COUNT = 2;

  enum class TextureType: uint8_t
  {
    None      = 0,
    SurfaceRT = 1,
    Allocated = 2
  };

  struct TextureHandlerInternal
  {
    TextureHandlerInternal()
    {
      as.handler = 0;
    }

    TextureHandlerInternal(const TextureHandle handler)
    {
      as.handler = (uint64_t)handler;
    }

    operator TextureHandle()
    {
      return (TextureHandle)as.handler;
    }

    union
    {
      uint64_t handler;
      struct
      {
        uint64_t id: 61;
        uint64_t type: 3;
      } typed;
    } as;
  };

  enum class BufferType: uint8_t
  {
    None    = 0,
    Device  = 1,
    Staging = 2
  };

  struct Buffer
  {
    vk::UniqueDeviceMemory memory;
    vk::UniqueBuffer buffer;
    int usage = 0;

    size_t blockSize = 0;
    size_t alignedBlockSize = 0;
    size_t maxDiscards = 0;
    size_t discards = 0;
    bool isFirstDiscard = true;

    inline size_t getConstOffset() const
    {
      return alignedBlockSize * discards;
    }

    inline size_t getAbsOffset(const size_t localOffset) const
    {
      return getConstOffset() + localOffset;
    }
  };

  struct Texture
  {
    vk::UniqueImageView depthView;
    eastl::vector<vk::UniqueImageView> viewsPerMip;
    vk::ImageViewType viewType;
    vk::UniqueDeviceMemory memory;
    vk::UniqueImage img;
    vk::Format format;

    int3 size = {0,0,0};
  };

  struct Sampler
  {
    vk::UniqueSampler sampler;
  };

  struct VulkanFence: public gapi::Fence
  {
    virtual ~VulkanFence() override {}
    virtual void wait() override
    {
      VK_CHECK(get_device().waitForFences(1, &fence.get(), true, ~0));
    }
    virtual void reset() override
    {
      VK_CHECK(get_device().resetFences(1, &fence.get()));
    }
    vk::UniqueFence fence;
  };
}