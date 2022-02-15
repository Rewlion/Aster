#pragma once

#include "resources.h"

namespace gapi
{

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

  struct PresentSurfaceImageCmd {};

  struct PushConstantsCmd
  {
    void*       data = nullptr;
    size_t      size = 0;
    ShaderStage stage = ShaderStage::Vertex;
  };

  using Command = std::variant<
    BeginRenderPassCmd,
    EndRenderPassCmd,
    BindGraphicsPipelineCmd,
    DrawCmd,
    PresentSurfaceImageCmd,
    PushConstantsCmd
  >;
  using CommandList = eastl::vector<Command>;

}
