#pragma once

#include <stdint.h>

#include <variant>
#include <EASTL/vector.h>

namespace gapi
{
  constexpr size_t MAX_RENDER_TARGETS = 8;

  enum class TextureHandler: uint64_t { Invalid = (uint64_t)-1 };
  enum class BufferHandler: uint64_t { Invalid = (uint64_t)-1 };

  enum class TextureLoadOp
  {
    Load = 0,
    Clear = 1,
    DontCare = 2
  };

  enum class TextureStoreOp
  {
    Store = 0,
    DontCare = 1
  };

  struct ColorAttachment
  {
    TextureHandler texture = TextureHandler::Invalid;
    TextureLoadOp  loadOp  = TextureLoadOp::DontCare;
    TextureStoreOp storeOp = TextureStoreOp::DontCare;
  };

  struct DepthStencilAttachment
  {
    TextureHandler texture        = TextureHandler::Invalid;
    TextureLoadOp  depthLoadOp    = TextureLoadOp::DontCare;
    TextureStoreOp depthStoreOp   = TextureStoreOp::DontCare;
    TextureLoadOp  stencilLoadOp  = TextureLoadOp::DontCare;
    TextureStoreOp stencilStoreOp = TextureStoreOp::DontCare;
  };

  struct BeginRenderPassCmd
  {
    ColorAttachment renderTargets[MAX_RENDER_TARGETS] = { TextureHandler::Invalid };
    DepthStencilAttachment depthStencil;

    size_t hash() const;
  };

  struct EndRenderPassCmd
  {
  };

  using Command = std::variant<BeginRenderPassCmd, EndRenderPassCmd>;
  using CommandList = eastl::vector<Command>;
}