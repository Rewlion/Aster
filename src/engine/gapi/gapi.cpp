#include "gapi.h"

#include <engine/assert.h>
#include <engine/gapi/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void                     (*gapi_wait_gpu_idle)();
  void                     (*gapi_shutdown)();
  TextureHandle            (*gapi_get_backbuffer)();
  DepthStencilStateHandler (*gapi_create_depth_stencil_state)(const DepthStencilStateDescription& desc);
  BufferHandler            (*gapi_allocate_buffer)(const size_t size, const int usage, const char* name);
  void                     (*gapi_free_buffer)(const BufferHandler buffer);
  void                     (*gapi_free_texture)(const TextureHandle texture);
  void                     (*gapi_free_sampler)(const SamplerHandler sampler);
  void                     (*gapi_free_pipeline_resources)();
  void*                    (*gapi_map_buffer)(const BufferHandler buffer, const size_t offset, const size_t size, const int flags);
  void                     (*gapi_unmap_buffer)(const BufferHandler buffer);
  TextureHandle            (*gapi_allocate_texture)(const TextureAllocationDescription& allocDesc);
  void                     (*gapi_bind_texture)(const TextureHandle texture, const size_t set, const size_t binding);
  SamplerHandler           (*gapi_allocate_sampler)(const SamplerAllocationDescription& allocDesc);
  void                     (*gapi_acquire_backbuffer)();
  ShaderModuleHandler      (*gapi_add_module)(void* blob);
  PipelineLayoutHandler    (*gapi_add_pipeline_layout)(void* dsets);
  CmdEncoder*              (*gapi_allocate_cmd_encoder)();
  Fence*                   (*gapi_allocate_fence)();
  CmdPromise*              (*gapi_allocate_cmd_promise)();
  void                     (*gapi_present_backbuffer_and_finalize_frame)();
  void                     (*gapi_wait_fence)(Fence* fence);

  void init()
  {
    const string& api = Engine::get_app_settings()->getText("gapi", "vulkan");
    if (api == "vulkan")
      vulkan::init();
    else
      ASSERT(!"unknown graphics api");
  }

  void wait_gpu_idle()
  {
    gapi_wait_gpu_idle();
  }

  void shutdown()
  {
    gapi_shutdown();
  }

  auto get_backbuffer() -> TextureHandle
  {
    return gapi_get_backbuffer();
  }

  auto create_depth_stencil_state(const DepthStencilStateDescription& desc) -> DepthStencilStateHandler
  {
    return gapi_create_depth_stencil_state(desc);
  }

  auto allocate_buffer(const size_t size, const int usage, const char* name) -> BufferHandler
  {
    return gapi_allocate_buffer(size, usage, name);
  }

  auto map_buffer(BufferHandler buffer, const size_t offset, const size_t size, const int flags) -> void*
  {
    return gapi_map_buffer(buffer, offset, size, flags);
  }

  void unmap_buffer(BufferHandler buffer)
  {
    return gapi_unmap_buffer(buffer);
  }

  auto allocate_texture(const TextureAllocationDescription& allocDesc) -> TextureHandle
  {
    return gapi_allocate_texture(allocDesc);
  }

  auto allocate_sampler(const SamplerAllocationDescription& allocDesc) -> SamplerHandler
  {
    return gapi_allocate_sampler(allocDesc);
  }

  void free_resource(const BufferHandler buffer)
  {
    gapi_free_buffer(buffer);
  }

  void free_texture(const TextureHandle texture)
  {
    gapi_free_texture(texture);
  }

  void free_resource(const TextureHandle texture)
  {
    gapi_free_texture(texture);
  }

  void free_pipeline_resources()
  {
    gapi_free_pipeline_resources();
  }

  void free_resource(const SamplerHandler sampler)
  {
    gapi_free_sampler(sampler);
  }

  void acquire_backbuffer()
  {
    gapi_acquire_backbuffer();
  }

  auto add_module(void* blob) -> ShaderModuleHandler
  {
    return gapi_add_module(blob);
  }

  auto add_pipeline_layout(void* dsets) -> PipelineLayoutHandler
  {
    return gapi_add_pipeline_layout(dsets);
  }

  auto allocate_cmd_encoder() -> CmdEncoder*
  {
    return gapi_allocate_cmd_encoder();
  }

  auto allocate_fence() -> Fence*
  {
    return gapi_allocate_fence();
  }

  auto allocate_cmd_promise() -> CmdPromise*
  {
    return gapi_allocate_cmd_promise();
  }

  void present_backbuffer_and_finalize_frame()
  {
    gapi_present_backbuffer_and_finalize_frame();
  }

  void wait_fence(Fence* fence)
  {
    gapi_wait_fence(fence);
  }
}
