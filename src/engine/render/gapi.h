#pragma once

#include "resources.h"

namespace gapi
{
  void init();
  void PresentSurfaceImage();
  void submitCommands(CommandList&& cmds);
  TextureHandler getCurrentSurfaceRT();
  DepthStencilStateHandler createDepthStencilState(const DepthStencilStateDescription& desc);
}
