#pragma once

#include "gapi.h"

#include <engine/assert.h>
#include <engine/render/vulkan/vulkan.h>
#include <engine/settings.h>

namespace gapi
{
  void init()
  {
    const String& api = get_app_settings()->get_text("gapi", "vulkan");
    if (api == "vulkan")
      vulkan::init();
    else
      ASSERT(!"unknown graphics api");
  }
}
