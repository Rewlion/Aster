#pragma once

#include "gapi.h"

#include <engine/assert.h>
#include <engine/render/gapi/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void (*gapiSubmitCommands)(CommandList&& cmds);
  TextureHandler (*gapiGetCurrentSurfaceRT)();
  DepthStencilStateHandler (*gapiCreateDepthStencilState)(const DepthStencilStateDescription& desc);
  BufferHandler (*gapiAllocateBuffer)(const BufferAllocationDescription&);
  void (*gapiCopyToBufferSync)(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);

  void init()
  {
    const string& api = Engine::GetAppSettings()->GetText("gapi", "vulkan");
    if (api == "vulkan")
      vulkan::init();
    else
      ASSERT(!"unknown graphics api");
  }

  void submitCommands(CommandList&& cmds)
  {
    gapiSubmitCommands(std::move(cmds));
  }

  TextureHandler getCurrentSurfaceRT()
  {
    return gapiGetCurrentSurfaceRT();
  }

  DepthStencilStateHandler createDepthStencilState(const DepthStencilStateDescription& desc)
  {
    return gapiCreateDepthStencilState(desc);
  }

  BufferHandler AllocateBuffer(const BufferAllocationDescription& allocDesc)
  {
    return gapiAllocateBuffer(allocDesc);
  }

  void CopyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer)
  {
    gapiCopyToBufferSync(src, offset, size, buffer);
  }
}
