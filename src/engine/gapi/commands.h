#pragma once

#include "resources.h"

#include <engine/utils/fixed_stack.hpp>

namespace gapi
{
  struct BeginRenderPassCmd
  {
    RenderTargets        renderTargets;
    RenderPassAttachment depthStencil;
  };

  struct BindGraphicsShadersCmd
  {
    ShaderStagesNames shaderNames;
  };

  struct DrawCmd
  {
    const PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    uint32_t                vertexCount = 0;
    uint32_t                instanceCount = 0;
    uint32_t                firstVertex = 0;
    uint32_t                firstInstance = 0;
  };

  struct DrawIndexedCmd
  {
    const PrimitiveTopology topology = PrimitiveTopology::TriangleList;
    uint32_t                indexCount    = 0;
    uint32_t                instanceCount = 0;
    uint32_t                firstIndex    = 0;
    uint32_t                vertexOffset  = 0;
    uint32_t                firstInstance = 0;
  };

  struct PresentSurfaceImageCmd {};

  struct PushConstantsCmd
  {
    const void* data = nullptr;
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

  struct BindConstantBufferCmd
  {
    BufferHandler buffer;
    size_t        argument = 0;
    size_t        binding  = 0;
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

  struct ClearCmd
  {
    int clearing = CLEAR_NONE;
  };

  struct SetBlendStateCmd
  {
    BlendState blending;
  };

  struct SetDepthStencilStateCmd
  {
    DepthStencilStateDescription ds;
  };

  using Command = std::variant<
    BeginRenderPassCmd,
    BindGraphicsShadersCmd,
    DrawCmd,
    DrawIndexedCmd,
    PresentSurfaceImageCmd,
    PushConstantsCmd,
    BindVertexBufferCmd,
    BindIndexBufferCmd,
    BindConstantBufferCmd,
    BindTextureCmd,
    BindSamplerCmd,
    ClearCmd,
    SetBlendStateCmd,
    SetDepthStencilStateCmd
  >;
  using CommandList = eastl::vector<Command>;
}
