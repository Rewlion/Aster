#pragma once

#include <engine/assert.h>
#include <engine/render/gapi/resources.h>

#include <vulkan/vulkan.hpp>
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

    TextureHandlerInternal(const TextureHandler handler)
    {
      as.handler = (uint64_t)handler;
    }

    operator TextureHandler()
    {
      return (TextureHandler)as.handler;
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
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
  };

  struct Texture
  {
    vk::UniqueImage img;
    vk::UniqueDeviceMemory memory;
    vk::UniqueImageView view;

    vk::ImageLayout currentLayout = vk::ImageLayout::eUndefined;
    int3 size = {0,0,0};
  };

  struct Sampler
  {
    vk::UniqueSampler sampler;
  };

  inline vk::AttachmentLoadOp loadOpToVk(const TextureLoadOp op)
  {
    switch (op)
    {
      case TextureLoadOp::Load: return vk::AttachmentLoadOp::eLoad;
      case TextureLoadOp::Clear: return vk::AttachmentLoadOp::eClear;
      case TextureLoadOp::DontCare: return vk::AttachmentLoadOp::eDontCare;
      default: ASSERT(!"unsupported load op"); return vk::AttachmentLoadOp::eDontCare;
    }
  }

  inline vk::AttachmentStoreOp storeOpToVk(const TextureStoreOp op)
  {
    switch (op)
    {
      case TextureStoreOp::Store: return vk::AttachmentStoreOp::eStore;
      case TextureStoreOp::DontCare: return vk::AttachmentStoreOp::eDontCare;
      default: ASSERT(!"unsupported load op"); return vk::AttachmentStoreOp::eDontCare;
    }
  }
}