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

  using Command = std::variant<
    BeginRenderPassCmd,
    EndRenderPassCmd,
    BindGraphicsPipelineCmd,
    DrawCmd,
    PresentSurfaceImageCmd,
    PushConstantsCmd,
    BindVertexBufferCmd
  >;
  using CommandList = eastl::vector<Command>;

}
