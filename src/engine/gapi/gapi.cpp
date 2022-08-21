#include "gapi.h"

#include <engine/assert.h>
#include <engine/gapi/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  TextureHandler           (*gapi_get_backbuffer)();
  DepthStencilStateHandler (*gapi_create_depth_stencil_state)(const DepthStencilStateDescription& desc);
  BufferHandler            (*gapi_allocate_buffer)(const size_t size, const int usage);
  void                     (*gapi_free_buffer)(const BufferHandler buffer);
  void                     (*gapi_free_texture)(const TextureHandler texture);
  void*                    (*gapi_map_buffer)(const BufferHandler buffer, const size_t offset, const size_t size, const int flags);
  void                     (*gapi_unmap_buffer)(const BufferHandler buffer);
  TextureHandler           (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  void                     (*gapi_bind_texture)(const TextureHandler texture, const size_t set, const size_t binding);
  SamplerHandler           (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);
  Fence*                   (*gapi_ackquire_backbuffer)();
  ShaderModuleHandler      (*gapi_add_module)(void* blob);
  PipelineLayoutHandler    (*gapi_add_pipeline_layout)(void* dsets);
  CmdEncoder*              (*gapi_allocate_cmd_encoder)();
  Fence*                   (*gapi_allocate_fence)();
  void                     (*gapi_next_frame)();
  void                     (*gapi_wait_fence)(Fence* fence);

  void init()
  {
    const string& api = Engine::get_app_settings()->getText("gapi", "vulkan");
    if (api == "vulkan")
      vulkan::init();
    else
      ASSERT(!"unknown graphics api");
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

  void* map_buffer(BufferHandler buffer, const size_t offset, const size_t size, const int flags)
  {
    return gapi_map_buffer(buffer, offset, size, flags);
  }

  void unmap_buffer(BufferHandler buffer)
  {
    return gapi_unmap_buffer(buffer);
  }

  TextureHandler allocate_texture(const TextureAllocationDescription& allocDesc)
  {
    return gapi_allocate_texture(allocDesc);
  }

  SamplerHandler allocate_sampler(const SamplerAllocationDescription& allocDesc)
  {
    return gapi_allocate_sampler(allocDesc);
  }

  void free_resource(const BufferHandler buffer)
  {
    gapi_free_buffer(buffer);
  }

  void free_texture(const TextureHandler texture)
  {
    gapi_free_texture(texture);
  }

  void free_resource(const TextureHandler texture)
  {
    gapi_free_texture(texture);
  }

  Fence* ackquire_backbuffer()
  {
    return gapi_ackquire_backbuffer();
  }

  ShaderModuleHandler add_module(void* blob)
  {
    return gapi_add_module(blob);
  }

  PipelineLayoutHandler add_pipeline_layout(void* dsets)
  {
    return gapi_add_pipeline_layout(dsets);
  }

  CmdEncoder* allocate_cmd_encoder()
  {
    return gapi_allocate_cmd_encoder();
  }

  Fence* allocate_fence()
  {
    return gapi_allocate_fence();
  }

  void next_frame()
  {
    gapi_next_frame();
  }

  void wait_fence(Fence* fence)
  {
    gapi_wait_fence(fence);
  }
}
