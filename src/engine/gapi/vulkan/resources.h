#pragma once

#include "vulkan.h"

#include "result.h"
#include "vma.h"

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
    VmaAllocator allocator = nullptr;
    VmaAllocation allocation = nullptr;
    vk::Buffer buffer = VK_NULL_HANDLE;
    int usage = 0;
    size_t blockSize = 0;

    Buffer() = default;
    Buffer(Buffer&& r)
    {
      *this = std::move(r);
    }
    
    Buffer& operator=(Buffer&& r)
    {
      this->~Buffer();
      std::swap(allocator, r.allocator);
      std::swap(allocation, r.allocation);
      std::swap(buffer, r.buffer);
      std::swap(usage, r.usage);
      std::swap(blockSize, r.blockSize);

      return *this;
    }

    ~Buffer()
    {
      if (buffer)
      {
        vmaDestroyBuffer(allocator, buffer, allocation);
        buffer = VK_NULL_HANDLE;
        allocation = nullptr;
      }
    }
  };

  struct Texture
  {
    vk::UniqueImageView depthView;
    vk::UniqueImageView srvView;
    eastl::vector<vk::UniqueImageView> viewsPerMip;
    vk::ImageViewType viewType;
    VmaAllocator allocator = nullptr;
    VmaAllocation allocation = nullptr;;
    vk::Image img = VK_NULL_HANDLE;
    vk::Format format = vk::Format::eUndefined;
    int3 size = {0,0,0};

    Texture() = default;
    Texture(Texture&& r)
    {
      *this = std::move(r);
    }
    Texture& operator=(Texture&& r)
    {
      this->~Texture();

      std::swap(depthView, r.depthView);
      std::swap(srvView, r.srvView);
      std::swap(viewsPerMip, r.viewsPerMip);
      std::swap(viewType, r.viewType);
      std::swap(allocator, r.allocator);
      std::swap(allocation, r.allocation);
      std::swap(img, r.img);
      std::swap(format, r.format);
      std::swap(size, r.size);

      return *this;
    }
    ~Texture()
    {
      if (img)
      {
        vmaDestroyImage(allocator, img, allocation);
        img = VK_NULL_HANDLE;
        allocator = nullptr;
      }
    }
  };

  struct Sampler
  {
    vk::UniqueSampler sampler;
  };

  struct VulkanFence: public gapi::Fence
  {
    ~VulkanFence() override {}
    void wait() override
    {
      VK_CHECK(get_device().waitForFences(1, &fence.get(), true, ~0));
    }
    void reset() override
    {
      VK_CHECK(get_device().resetFences(1, &fence.get()));
    }
    vk::UniqueFence fence;
  };

  struct VulkanCmdPromise: public gapi::CmdPromise
  {
    ~VulkanCmdPromise() override
    {
      extern void free_cmd_promise(VulkanCmdPromise* promise);
      free_cmd_promise(this);
    }

    auto isReady() const -> bool override
    {
      auto res = get_device().getEventStatus(event.get());
      return res == vk::Result::eEventSet;
    }

    void reset() override
    {
      get_device().resetEvent(event.get());
    }

    vk::UniqueEvent event;
  };
}