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
    Load     = 0,
    Clear    = 1,
    DontCare = 2
  };

  enum class TextureStoreOp
  {
    Store    = 0,
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
    Never          = 0,
    Less           = 1,
    Equal          = 2,
    LessOrEqual    = 3,
    Greater        = 4,
    NotEqual       = 5,
    GreaterOrEqual = 6,
    Always         = 7
  };

  enum class StencilOp: uint8_t
  {
    Keep              = 0,
    Zero              = 1,
    Replace           = 2,
    IncrementAndClamp = 3,
    DecrementAndClamp = 4,
    Invert            = 5,
    IncrementAndWrap  = 6,
    DecrementAndWrap  = 7
  };

  struct StencilOpState
  {
    StencilOp failOp         = StencilOp::Keep;
    StencilOp passOp         = StencilOp::Keep;
    StencilOp depthFailOp    = StencilOp::Keep;
    CompareOp compareOp      = CompareOp::Never;
    uint32_t  referenceValue = 0;
  };

  struct DepthStencilStateDescription
  {
    bool      depthTestEnabled      = false;
    bool      depthWriteEnabled     = false;
    CompareOp depthOp               = CompareOp::Never;
    bool      stencilTestEnabled    = false;
    StencilOp stencilFailOp         = StencilOp::Keep;
    StencilOp stencilPassOp         = StencilOp::Keep;
    StencilOp stencilDepthFailOp    = StencilOp::Keep;
    CompareOp stencilCompareOp      = CompareOp::Never;
    uint32_t  stencilReferenceValue = 0;

    size_t hash() const;
  };

  enum class PrimitiveTopology: uint8_t
  {
    PointList                  = 0,
    LineList                   = 1,
    LineStrip                  = 2,
    TriangleList               = 3,
    TriangleStrip              = 4,
    TriangleFan                = 5,
    LineListWithAdjacency      = 6,
    LineStripWithAdjacency     = 7,
    TriangleListWithAdjacency  = 8,
    TriangleStripWithAdjacency = 9,
    PathList                   = 10
  };

  struct GraphicsPipelineLayoutDescription
  {
    inline size_t hash() const { return 0; };
  };

  enum class LogicOp: uint8_t
  {
    Clear        = 0,
    And          = 1,
    AndReverse   = 2,
    Copy         = 3,
    AndInverted  = 4,
    NoOp         = 5,
    Xor          = 6,
    Or           = 7,
    Nor          = 8,
    Equivalent   = 9,
    Invert       = 10,
    OrReverse    = 11,
    CopyInverted = 12,
    OrInverted   = 13,
    Nand         = 14,
    Set          = 15
  };

  enum class BlendFactor: uint8_t
  {
    Zero                  = 0,
    One                   = 1,
    SrcColor              = 2,
    OneMinusSrcColor      = 3,
    DstColor              = 4,
    OneMinusDstColor      = 5,
    SrcAlpha              = 6,
    OneMinusSrcAlpha      = 7,
    DstAlpha              = 8,
    OneMinusDstAlpha      = 9,
    ConstantColor         = 10,
    OneMinusConstantColor = 11,
    ConstantAlpha         = 12,
    OneMinusConstantAlpha = 13,
    SrcAlphaSaturate      = 14,
    Src1Color             = 15,
    OneMinusSrc1Color     = 16,
    Src1Alpha             = 17,
    OneMinusSrc1Alpha     = 18
  };

  enum class BlendOp: uint8_t
  {
    Add             = 0,
    Subtract        = 1,
    ReverseSubtract = 2,
    Min             = 3,
    Max             = 4
  };

  struct AttachmentBlendState
  {
    bool        blendEnabled        = false;
    BlendFactor srcColorBlendFactor = BlendFactor::Zero;
    BlendFactor dstColorBlendFactor = BlendFactor::Zero;
    BlendOp     colorBlendOp        = BlendOp::Add;
    BlendFactor srcAlphaBlendFactor = BlendFactor::Zero;
    BlendFactor dstAlphaBlendFactor = BlendFactor::Zero;
    BlendOp     alphaBlendOp        = BlendOp::Add;

    size_t hash() const;
  };

  struct BlendState
  {
    bool                 logicOpEnabled   = false;
    LogicOp              logicOp          = LogicOp::Clear;
    size_t               attachmentsCount = 0;
    AttachmentBlendState attachments[MAX_RENDER_TARGETS];
    float4               blendConstants   = float4{0.0f , 0.0f, 0.0f, 0.0f};

    size_t hash() const;
  };

  struct GraphicsPipelineDescription
  {
    string_hash shadersNames[2];
    size_t shadersCount = 0;
    PrimitiveTopology topology;
    DepthStencilStateDescription depthStencilState;
    BlendState blendState;

    size_t hash() const;
  };

}
