#pragma once

#include "gapi.h"

#include <engine/assert.h>
#include <engine/render/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void (*gapiSubmitCommands)(CommandList&& cmds);
  TextureHandler (*gapiGetCurrentSurfaceRT)();
  void (*gapiPresentSurfaceImage)();
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

  void PresentSurfaceImage()
  {
    gapiPresentSurfaceImage();
  }

  DepthStencilStateHandler createDepthStencilState(const DepthStencilStateDescription& desc)
  {
    return gapiCreateDepthStencilState(desc);
  }
}
