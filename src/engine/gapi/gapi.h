#pragma once

#include "resources.h"
#include "commands.h"

namespace gapi
{
  void                     init();
  void                     submit_commands(CommandList&& cmds);
  TextureHandler           get_backbuffer();
  DepthStencilStateHandler create_depth_stencil_state(const DepthStencilStateDescription& desc);
  BufferHandler            allocate_buffer(const size_t size, const int usage);
  void*                    map_buffer(const BufferHandler buffer, const size_t offset, const size_t size);
  void                     unmap_buffer(const BufferHandler buffer);
  void                     copy_buffers_sync(const BufferHandler src, const size_t srcOffset, const BufferHandler dst, const size_t dstOffset, const size_t size);
  void                     write_buffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags = 0);
  TextureHandler           allocate_texture(const TextureAllocationDescription& allocDesc);
  void                     copy_to_texture_sync(const void* src, const size_t size, const TextureHandler texture);
  SamplerHandler           allocate_sampler(const SamplerAllocationDescription& allocDesc);
}
