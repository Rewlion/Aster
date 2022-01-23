#pragma once

#include "vulkan.h"
#include "backend.h"
#include "compile_context.h"
#include "resources.h"

#include <engine/render/vulkan/cache/shaders_storage.h>
#include <engine/render/gapi.h>

#include <vulkan/vulkan.hpp>

namespace gapi
{
  extern void (*gapiSubmitCommands)(CommandList&& cmds);
  extern TextureHandler (*gapiGetCurrentSurfaceRT)();
  extern void (*gapiBeginFrame)();
  extern void (*gapiEndFrame)();
}

namespace gapi::vulkan
{
  static Backend backend;
  static Device device;
  static ShadersStorage pipelineStorage;
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

  void beginFrame()
  {
    device.beginFrame();
  }

  void endFrame()
  {
    device.endFrame();
  }

  void init()
  {
    gapiSubmitCommands = submitCommands;
    gapiGetCurrentSurfaceRT = getCurrentSurfaceRT;
    gapiBeginFrame = beginFrame;
    gapiEndFrame = endFrame;

    backend.Init();
    device = backend.CreateDevice();
    pipelineStorage.Init(&device);
    compileContext.init(&device);
  }

}