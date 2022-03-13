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
  extern void           (*gapiSubmitCommands)(CommandList&& cmds);
  extern TextureHandler (*gapiGetCurrentSurfaceRT)();
  extern BufferHandler  (*gapiAllocateBuffer)(const BufferAllocationDescription&);
  extern void           (*gapiCopyToBufferSync)(const void* src, const size_t offset, const size_t size, const BufferHandler buffer);
  extern TextureHandler (*gapiAllocateTexture)(const TextureAllocationDescription& allocDesc);
  extern void           (*gapiCopyToTextureSync)(const void* src, const size_t size, const TextureHandler texture);
  extern SamplerHandler (*gapiAllocateSampler)(const SamplerAllocationDescription& allocDesc);
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

  BufferHandler AllocateBuffer(const BufferAllocationDescription& allocDesc)
  {
    return device.AllocateBuffer(allocDesc);
  }

  void CopyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer)
  {
    device.CopyToBufferSync(src, offset, size, buffer);
  }

  TextureHandler AllocateTexture(const TextureAllocationDescription& allocDesc)
  {
    return device.AllocateTexture(allocDesc);
  }

  void CopyToTextureSync(const void* src, const size_t size, const TextureHandler texture)
  {
    device.CopyToTextureSync(src, size, texture);
  }

  SamplerHandler AllocateSampler(const SamplerAllocationDescription& allocDesc)
  {
    return device.AllocateSampler(allocDesc);
  }

  void init()
  {
    gapiSubmitCommands = submitCommands;
    gapiGetCurrentSurfaceRT = getCurrentSurfaceRT;
    gapiAllocateBuffer = AllocateBuffer;
    gapiCopyToBufferSync = CopyToBufferSync;
    gapiAllocateTexture = AllocateTexture;
    gapiCopyToTextureSync = CopyToTextureSync;
    gapiAllocateSampler = AllocateSampler;

    backend.Init();
    device = backend.CreateDevice();
    compileContext.init(&device);
  }

}