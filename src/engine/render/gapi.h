#pragma once

#include "resources.h"

namespace gapi
{
  void init();
  void beginFrame();
  void endFrame();
  void submitCommands(CommandList&& cmds);
  TextureHandler getCurrentSurfaceRT();
  DepthStencilStateHandler createDepthStencilState(const DepthStencilStateDescription& desc);
}
