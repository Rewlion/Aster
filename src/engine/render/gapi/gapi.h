#pragma once

#include "resources.h"
#include "commands.h"

namespace gapi
{
  void                     init();
  void                     submit_commands(CommandList&& cmds);
  TextureHandler           get_backbuffer();
  DepthStencilStateHandler create_depth_stencil_state(const DepthStencilStateDescription& desc);
  BufferHandler            allocate_buffer(const BufferAllocationDescription& allocDesc);
  void                     copy_to_buffer_sync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);
  TextureHandler           allocate_texture(const TextureAllocationDescription& allocDesc);
  void                     copy_to_texture_sync(const void* src, const size_t size, const TextureHandler texture);
  SamplerHandler           allocate_sampler(const SamplerAllocationDescription& allocDesc);
}
