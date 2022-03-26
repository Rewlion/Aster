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
  extern BufferHandler  (*gapi_allocate_buffer)(const BufferAllocationDescription&);
  extern void           (*gapi_copy_to_buffer_sync)(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);
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

  BufferHandler allocate_buffer(const BufferAllocationDescription& allocDesc)
  {
    return device.allocateBuffer(allocDesc);
  }

  void copy_to_buffer_sync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer)
  {
    device.copyToBufferSync(src, offset, size, buffer);
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
    gapi_copy_to_buffer_sync = copy_to_buffer_sync;
    gapi_allocate_texture = allocate_texture;
    gapi_copy_to_texture_sync = copy_to_texture_sync;
    gapi_allocate_sampler = allocate_sampler;

    backend.init();
    device = backend.createDevice();
    compileContext.init(&device);
  }

}