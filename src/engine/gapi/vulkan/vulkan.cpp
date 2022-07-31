#pragma once

#include "vulkan.h"
#include "backend.h"
#include "cmd_encoder.h"
#include "frame_gc.h"
#include "resources.h"
#include "result.h"

#include <engine/gapi/vulkan/cache/pipelines_storage.h>
#include <engine/gapi/vulkan/cache/renderpass_storage.h>
#include <engine/gapi/gapi.h>

namespace gapi
{
  extern TextureHandler       (*gapi_get_backbuffer)();
  extern BufferHandler        (*gapi_allocate_buffer)(const size_t size, const int usage);
  extern void                 (*gapi_free_buffer)(const BufferHandler buffer);
  extern void                 (*gapi_free_texture)(const TextureHandler texture);
  extern void*                (*gapi_map_buffer)(const BufferHandler buffer, const size_t offset, const size_t size);
  extern void                 (*gapi_unmap_buffer)(const BufferHandler buffer);
  extern void                 (*gapi_copy_buffers_sync)(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size);
  extern void                 (*gapi_write_buffer)(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags);
  extern TextureHandler       (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  extern void                 (*gapi_copy_to_texture_sync)(const void* src, const size_t size, const TextureHandler texture);
  extern SamplerHandler       (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);
  extern void                 (*gapi_present_backbuffer)();
  extern Semaphore*           (*gapi_ackquire_backbuffer)();
  extern ShaderModuleHandler  (*gapi_add_module)(void* blob, void* reflection);
  extern gapi::CmdEncoder*    (*gapi_allocate_cmd_encoder)();
  extern Fence*               (*gapi_allocate_fence)();
}

namespace gapi::vulkan
{
  static Backend backend;
  static Device device;
  static FrameGarbageCollector frameGc;
  static PipelinesStorage pipelinesStorage;
  static RenderPassStorage rpStorage;

  vk::Device& get_device()
  {
    return device.getDevice();
  }

  TextureHandler get_backbuffer()
  {
    return device.getBackbuffer();
  }

  BufferHandler allocate_buffer(const size_t size, const int usage)
  {
    return device.allocateBuffer(size, usage);
  }

  void free_buffer(const BufferHandler buffer)
  {
    device.freeBuffer(buffer);
  }

  void free_texture(const TextureHandler texture)
  {
    device.freeTexture(texture);
  }

  void* map_buffer(const BufferHandler buffer, const size_t offset, const size_t size)
  {
    return device.mapBuffer(buffer, offset, size);
  }

  void unmap_buffer(const BufferHandler buffer)
  {
    device.unmapBuffer(buffer);
  }

  void copy_buffers_sync(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size)
  {
    device.copyBuffersSync(src, srcOffset, dst, dstOffset, size);
  }

  void write_buffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags)
  {
    device.writeBuffer(buffer, src, offset, size, flags);
  }

  TextureHandler allocate_texture(const TextureAllocationDescription& allocDesc)
  {
    return device.allocateTexture(allocDesc);
  }

  void copy_to_texture_sync(const void* src, const size_t size, const TextureHandler texture)
  {
    device.copyToTextureSync(src, size, texture);
  }

  SamplerHandler allocate_sampler(const SamplerAllocationDescription& allocDesc)
  {
    return device.allocateSampler(allocDesc);
  }

  void present_backbuffer()
  {
    device.presentSurfaceImage();
    frameGc.nextFrame();
  }

  Semaphore* ackquire_backbuffer()
  {
    VulkanSemaphore* s = new VulkanSemaphore();
    s->semaphore = device.createSemaphore();
    device.acquireBackbuffer(s->semaphore.get());
    return s;
  }

  ShaderModuleHandler add_module(void* blob, void* reflection)
  {
    const ShadersSystem::ShaderBlob* b = reinterpret_cast<const ShadersSystem::ShaderBlob*>(blob);
    const spirv::v2::Reflection* r = reinterpret_cast<const spirv::v2::Reflection*>(reflection);
    return pipelinesStorage.addModule(*b, *r);
  }

  gapi::CmdEncoder* allocate_cmd_encoder()
  {
    vk::UniqueCommandPool cmdPool = device.allocateCmdPool();
    return new CmdEncoder(device, frameGc, rpStorage, pipelinesStorage, std::move(cmdPool));
  }

  Fence* allocate_fence()
  {
    VulkanFence* f = new VulkanFence{};
    auto fence = device.getDevice().createFenceUnique(vk::FenceCreateInfo{});
    VK_CHECK_RES(fence);

    f->fence = std::move(fence.value);
    VK_CHECK(device.getDevice().resetFences(1, &f->fence.get()));
    return f;
  }

  void init()
  {
    gapi_get_backbuffer = get_backbuffer;
    gapi_allocate_buffer = allocate_buffer;
    gapi_free_buffer = free_buffer;
    gapi_free_texture = free_texture;
    gapi_allocate_texture = allocate_texture;
    gapi_copy_to_texture_sync = copy_to_texture_sync;
    gapi_allocate_sampler = allocate_sampler;
    gapi_map_buffer = map_buffer;
    gapi_unmap_buffer = unmap_buffer;
    gapi_copy_buffers_sync = copy_buffers_sync;
    gapi_write_buffer = write_buffer;
    gapi_present_backbuffer = present_backbuffer;
    gapi_ackquire_backbuffer = ackquire_backbuffer;
    gapi_add_module = add_module;
    gapi_allocate_cmd_encoder = allocate_cmd_encoder;
    gapi_allocate_fence = allocate_fence;

    backend.init();
    device = backend.createDevice(&frameGc);
    frameGc.init(&device);
    rpStorage.init(&device);
    pipelinesStorage.init(&device);
  }

}