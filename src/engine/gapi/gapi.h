#pragma once

#include "resources.h"

namespace gapi
{
  class CmdEncoder;

  void                     init();
  TextureHandler           get_backbuffer();
  DepthStencilStateHandler create_depth_stencil_state(const DepthStencilStateDescription& desc);
  BufferHandler            allocate_buffer(const size_t size, const int usage);
  void*                    map_buffer(const BufferHandler buffer, const size_t offset, const size_t size, const int flags);
  void                     unmap_buffer(const BufferHandler buffer);
  TextureHandler           allocate_texture(const TextureAllocationDescription& allocDesc);
  SamplerHandler           allocate_sampler(const SamplerAllocationDescription& allocDesc);
  void                     free_resource(const BufferHandler buffer);
  void                     free_resource(const TextureHandler texture);
  Fence*                   ackquire_backbuffer();
  ShaderModuleHandler      add_module(void* blob);
  PipelineLayoutHandler    add_pipeline_layout(void* dsets);
  CmdEncoder*              allocate_cmd_encoder();
  Fence*                   allocate_fence();
  void                     next_frame();
  void                     wait_fence(Fence* fence);
}
