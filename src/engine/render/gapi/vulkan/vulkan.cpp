#pragma once

#include "vulkan.h"
#include "backend.h"
#include "compile_context.h"
#include "resources.h"

#include <engine/render/gapi/vulkan/cache/pipelines_storage.h>
#include <engine/render/gapi/gapi.h>

#include <vulkan/vulkan.hpp>

namespace gapi
{
  extern void           (*gapi_submit_commands)(CommandList&& cmds);
  extern TextureHandler (*gapi_get_backbuffer)();
  extern BufferHandler  (*gapi_allocate_buffer)(const size_t size, const int usage);
  extern void*          (*gapi_map_buffer)(const BufferHandler buffer, const size_t offset, const size_t size);
  extern void           (*gapi_unmap_buffer)(const BufferHandler buffer);
  extern void           (*gapi_copy_buffers_sync)(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size);
  extern void           (*gapi_write_buffer)(const BufferHandler buffer, const void* src, const size_t offset, const size_t size);
  extern TextureHandler (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  extern void           (*gapi_copy_to_texture_sync)(const void* src, const size_t size, const TextureHandler texture);
  extern SamplerHandler (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);
}

namespace gapi::vulkan
{
  static Backend backend;
  static Device device;
  static CompileContext compileContext;

  void submit_commands(CommandList&& cmds)
  {
    for(const Command& cmd: cmds)
    {
      std::visit(
        [](const auto& c)
        {
          compileContext.compileCommand(c);
        },
        cmd);
    }
  }

  TextureHandler get_backbuffer()
  {
    TextureHandlerInternal h;
    h.as.typed.id = device.getBackbufferId();
    h.as.typed.type = (uint64_t)TextureType::SurfaceRT;

    return h;
  }

  void present_surface_image()
  {
    device.presentSurfaceImage();
  }

  BufferHandler allocate_buffer(const size_t size, const int usage)
  {
    return device.allocateBuffer(size, usage);
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

  void write_buffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size)
  {
    device.writeBuffer(buffer, src, offset, size);
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

  void init()
  {
    gapi_submit_commands = submit_commands;
    gapi_get_backbuffer = get_backbuffer;
    gapi_allocate_buffer = allocate_buffer;
    gapi_allocate_texture = allocate_texture;
    gapi_copy_to_texture_sync = copy_to_texture_sync;
    gapi_allocate_sampler = allocate_sampler;
    gapi_map_buffer = map_buffer;
    gapi_unmap_buffer = unmap_buffer;
    gapi_copy_buffers_sync = copy_buffers_sync;
    gapi_write_buffer = write_buffer;

    backend.init();
    device = backend.createDevice();
    compileContext.init(&device);
  }

}