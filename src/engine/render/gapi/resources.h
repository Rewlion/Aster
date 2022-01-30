#pragma once

#include <engine/types.h>

#include <variant>
#include <EASTL/vector.h>

namespace gapi
{
  constexpr size_t MAX_RENDER_TARGETS = 8;

  enum class TextureHandler: uint64_t { Invalid = (uint64_t)-1 };
  enum class BufferHandler: uint64_t { Invalid = (uint64_t)-1 };
  enum class DepthStencilStateHandler: uint64_t { Invalid = (uint64_t)-1 };

  enum class ShaderStage: uint8_t
  {
    None     = 0,
    Vertex   = 1,
    Fragment = 2,
  };

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

  enum class CompareOp: uint8_t
  {
    Never = 0,
    Less = 1,
    Equal = 2,
    LessOrEqual = 3,
    Greater = 4,
    NotEqual = 5,
    GreaterOrEqual = 6,
    Always = 7
  };

  enum class StencilOp: uint8_t
  {
    Keep = 0,
    Zero = 1,
    Replace = 2,
    IncrementAndClamp = 3,
    DecrementAndClamp = 4,
    Invert = 5,
    IncrementAndWrap = 6,
    DecrementAndWrap = 7
  };

  struct StencilOpState
  {
    StencilOp failOp = StencilOp::Keep;
    StencilOp passOp = StencilOp::Keep;
    StencilOp depthFailOp = StencilOp::Keep;
    CompareOp compareOp = CompareOp::Never;
    uint32_t referenceValue = 0;
  };

  struct DepthStencilStateDescription
  {
    bool depthTestEnabled = false;
    bool depthWriteEnabled = false;
    CompareOp depthOp = CompareOp::Never;

    bool stencilTestEnabled = false;
    StencilOp stencilFailOp = StencilOp::Keep;
    StencilOp stencilPassOp = StencilOp::Keep;
    StencilOp stencilDepthFailOp = StencilOp::Keep;
    CompareOp stencilCompareOp = CompareOp::Never;
    uint32_t stencilReferenceValue = 0;

    size_t hash() const;
  };

  enum class PrimitiveTopology: uint8_t
  {
    PointList = 0,
    LineList = 1,
    LineStrip = 2,
    TriangleList = 3,
    TriangleStrip = 4,
    TriangleFan = 5,
    LineListWithAdjacency = 6,
    LineStripWithAdjacency = 7,
    TriangleListWithAdjacency = 8,
    TriangleStripWithAdjacency = 9,
    PathList = 10
  };

  struct GraphicsPipelineDescription
  {
    string_hash shadersNames[2];
    size_t shadersCount = 0;
    PrimitiveTopology topology;
    DepthStencilStateDescription depthStencilState;

    size_t hash() const;
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

  struct BindGraphicsPipelineCmd
  {
    GraphicsPipelineDescription description;
  };

  struct DrawCmd
  {
    uint32_t vertexCount = 0;
    uint32_t instanceCount = 0;
    uint32_t firstVertex = 0;
    uint32_t firstInstance = 0;
  };

  using Command = std::variant<
    BeginRenderPassCmd,
    EndRenderPassCmd,
    BindGraphicsPipelineCmd,
    DrawCmd
  >;
  using CommandList = eastl::vector<Command>;
}
