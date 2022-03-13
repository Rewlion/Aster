#pragma once

#include "resources.h"
#include "commands.h"

namespace gapi
{
  void                     init();
  void                     submitCommands(CommandList&& cmds);
  TextureHandler           getCurrentSurfaceRT();
  DepthStencilStateHandler createDepthStencilState(const DepthStencilStateDescription& desc);
  BufferHandler            AllocateBuffer(const BufferAllocationDescription& allocDesc);
  void                     CopyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);
  TextureHandler           AllocateTexture(const TextureAllocationDescription& allocDesc);
  void                     CopyToTextureSync(const void* src, const size_t size, const TextureHandler texture);
  SamplerHandler           AllocateSampler(const SamplerAllocationDescription& allocDesc);
}
