#pragma once

#include "resources.h"

namespace gapi
{
  class CmdEncoder;

  void init();
  void wait_gpu_idle();
  void shutdown();
  auto get_backbuffer() -> TextureHandle;
  auto create_depth_stencil_state(const DepthStencilStateDescription& desc) -> DepthStencilStateHandler;
  auto allocate_buffer(const size_t size, const int usage, const char* name = "") -> BufferHandler;
  auto map_buffer(const BufferHandler buffer, const size_t offset, const size_t size, const int flags) -> void*;
  void unmap_buffer(const BufferHandler buffer);
  auto allocate_texture(const TextureAllocationDescription& allocDesc) -> TextureHandle;
  auto allocate_sampler(const SamplerAllocationDescription& allocDesc) -> SamplerHandler;
  void free_resource(const BufferHandler buffer);
  void free_resource(const TextureHandle texture);
  void free_resource(const SamplerHandler sampler);
  void free_pipeline_resources();
  void acquire_backbuffer();
  auto add_module(void* blob) -> ShaderModuleHandler;
  auto add_pipeline_layout(void* dsets) -> PipelineLayoutHandler;
  auto allocate_cmd_encoder() -> CmdEncoder*;
  auto allocate_fence() -> Fence*;
  void present_backbuffer_and_finalize_frame();
  void wait_fence(Fence* fence);
}
