#pragma once

#include "gapi.h"

#include <engine/assert.h>
#include <engine/render/gapi/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void                     (*gapi_submit_commands)(CommandList&& cmds);
  TextureHandler           (*gapi_get_backbuffer)();
  DepthStencilStateHandler (*gapi_create_depth_stencil_state)(const DepthStencilStateDescription& desc);
  BufferHandler            (*gapi_allocate_buffer)(const size_t size, const int usage);
  void*                    (*gapi_map_buffer)(const BufferHandler buffer, const size_t offset, const size_t size);
  void                     (*gapi_unmap_buffer)(const BufferHandler buffer);
  void                     (*gapi_copy_buffers_sync)(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size);
  void                     (*gapi_write_buffer)(const BufferHandler buffer, const void* src, const size_t offset, const size_t size);
  TextureHandler           (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  void                     (*gapi_copy_to_texture_sync)(const void* src, const size_t size, const TextureHandler texture);
  void                     (*gapi_bind_texture)(const TextureHandler texture, const size_t set, const size_t binding);
  SamplerHandler           (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);

  void init()
  {
    const string& api = Engine::get_app_settings()->getText("gapi", "vulkan");
    if (api == "vulkan")
      vulkan::init();
    else
      ASSERT(!"unknown graphics api");
  }

  void submit_commands(CommandList&& cmds)
  {
    gapi_submit_commands(std::move(cmds));
  }

  TextureHandler get_backbuffer()
  {
    return gapi_get_backbuffer();
  }

  DepthStencilStateHandler create_depth_stencil_state(const DepthStencilStateDescription& desc)
  {
    return gapi_create_depth_stencil_state(desc);
  }

  BufferHandler allocate_buffer(const size_t size, const int usage)
  {
    return gapi_allocate_buffer(size, usage);
  }

  void* map_buffer(BufferHandler buffer, const size_t offset, const size_t size)
  {
    return gapi_map_buffer(buffer, offset, size);
  }

  void unmap_buffer(BufferHandler buffer)
  {
    return gapi_unmap_buffer(buffer);
  }

  void copy_buffers_sync(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size)
  {
    gapi_copy_buffers_sync(src, srcOffset, dst, dstOffset, size);
  }

  void write_buffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size)
  {
    gapi_write_buffer(buffer, src, offset, size);
  }

  TextureHandler allocate_texture(const TextureAllocationDescription& allocDesc)
  {
    return gapi_allocate_texture(allocDesc);
  }

  void copy_to_texture_sync(const void* src, const size_t size, const TextureHandler texture)
  {
    gapi_copy_to_texture_sync(src, size, texture);
  }

  SamplerHandler allocate_sampler(const SamplerAllocationDescription& allocDesc)
  {
    return gapi_allocate_sampler(allocDesc);
  }

}
