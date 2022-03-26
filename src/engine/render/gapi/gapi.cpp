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
  BufferHandler            (*gapi_allocate_buffer)(const BufferAllocationDescription&);
  void                     (*gapi_copy_to_buffer_sync)(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);
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

  BufferHandler allocate_buffer(const BufferAllocationDescription& allocDesc)
  {
    return gapi_allocate_buffer(allocDesc);
  }

  void copy_to_buffer_sync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer)
  {
    gapi_copy_to_buffer_sync(src, offset, size, buffer);
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
