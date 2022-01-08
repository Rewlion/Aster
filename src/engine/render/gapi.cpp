#pragma once

#include "gapi.h"

#include <engine/assert.h>
#include <engine/render/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void (*gapiSubmitCommands)(CommandList&& cmds);
  TextureHandler (*gapiGetCurrentSurfaceRT)();
  void (*gapiBeginFrame)();
  void (*gapiEndFrame)();

  void init()
  {
    const String& api = get_app_settings()->get_text("gapi", "vulkan");
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

  void beginFrame()
  {
    gapiBeginFrame();
  }

  void endFrame()
  {
    gapiEndFrame();
  }
}
