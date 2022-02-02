#pragma once

#include "vulkan.h"
#include "backend.h"
#include "compile_context.h"
#include "resources.h"

#include <engine/render/gapi/vulkan/cache/pipelines_storage.h>
#include <engine/render/gapi/gapi.h>

#include <vulkan/vulkan.hpp>

namespace gapi
{
  extern void (*gapiSubmitCommands)(CommandList&& cmds);
  extern TextureHandler (*gapiGetCurrentSurfaceRT)();
}

namespace gapi::vulkan
{
  static Backend backend;
  static Device device;
  static CompileContext compileContext;

  void submitCommands(CommandList&& cmds)
  {
    for(const Command& cmd: cmds)
    {
      std::visit(
        [](const auto& c)
        {
          compileContext.compileCommand(c);
        },
        cmd);
    }
  }

  TextureHandler getCurrentSurfaceRT()
  {
    TextureHandlerInternal h;
    h.as.typed.id = device.getSurfaceRtId();
    h.as.typed.type = (uint64_t)TextureType::SurfaceRT;

    return h;
  }

  void PresentSurfaceImage()
  {
    device.PresentSurfaceImage();
  }

  void init()
  {
    gapiSubmitCommands = submitCommands;
    gapiGetCurrentSurfaceRT = getCurrentSurfaceRT;

    backend.Init();
    device = backend.CreateDevice();
    compileContext.init(&device);
  }

}