#pragma once

#include "resources.h"

#include <engine/utils/fixed_stack.hpp>

namespace gapi
{

  struct BeginRenderPassCmd
  {
    Utils::FixedStack<ColorAttachment, MAX_RENDER_TARGETS> renderTargets;
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

  struct PresentSurfaceImageCmd {};

  struct PushConstantsCmd
  {
    void*       data = nullptr;
    size_t      size = 0;
    ShaderStage stage = ShaderStage::Vertex;
  };

  struct BindVertexBufferCmd
  {
    BufferHandler buffer;
  };

  struct BindIndexBufferCmd
  {
    BufferHandler buffer;
  };

  struct DrawIndexedCmd
  {
    uint32_t indexCount    = 0;
    uint32_t instanceCount = 0;
    uint32_t firstIndex    = 0;
    uint32_t vertexOffset  = 0;
    uint32_t firstInstance = 0;
  };

  struct BindTextureCmd
  {
    TextureHandler texture  = TextureHandler::Invalid;
    size_t         argument = 0;
    size_t         binding  = 0;
  };

  struct BindSamplerCmd
  {
    SamplerHandler sampler  = SamplerHandler::Invalid;
    size_t         argument = 0;
    size_t         binding  = 0;
  };

  using Command = std::variant<
    BeginRenderPassCmd,
    EndRenderPassCmd,
    BindGraphicsPipelineCmd,
    DrawCmd,
    PresentSurfaceImageCmd,
    PushConstantsCmd,
    BindVertexBufferCmd,
    BindIndexBufferCmd,
    DrawIndexedCmd,
    BindTextureCmd,
    BindSamplerCmd
  >;
  using CommandList = eastl::vector<Command>;

}
