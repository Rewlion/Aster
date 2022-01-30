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
}
