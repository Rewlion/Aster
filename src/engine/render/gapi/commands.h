#pragma once

#include "resources.h"

#include <engine/utils/fixed_stack.hpp>

namespace gapi
{
  struct BeginRenderPassCmd
  {
    RenderTargets  renderTargets;
    TextureHandler depthStencil = TextureHandler::Invalid;
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
    ClearState clearing = CLEAR_NONE;
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
    BindTextureCmd,
    BindSamplerCmd,
    ClearCmd,
    SetBlendStateCmd,
    SetDepthStencilStateCmd
  >;
  using CommandList = eastl::vector<Command>;

  void BeginRenderPass(CommandList& cmdList, const RenderTargets& renderTargets, TextureHandler depthStencil = TextureHandler::Invalid);

  void BindGraphicsShaders(CommandList& cmdList, const ShaderStagesNames& stages);

  void Draw(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t vertexCount,
               const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance);

  void DrawIndexed(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t indexCount, uint32_t instanceCount,
                      const uint32_t firstIndex, const uint32_t vertexOffset, const uint32_t firstInstace);

  void PresentSurfaceImage(CommandList& cmdList);

  void PushConstants(CommandList& cmdList, const void* data, const size_t size, const ShaderStage stage);

  void BindVertexBuffer(CommandList& cmdList, const BufferHandler buffer);

  void BindIndexBuffer(CommandList& cmdList, const BufferHandler buffer);

  void BindTexture(CommandList& cmdList, const TextureHandler texture, const size_t argument, const size_t binding);

  void BindSampler(CommandList& cmdList, const SamplerHandler sampler, const size_t argument, const size_t binding);

  void Clear(CommandList& cmdList, const ClearState clearing);

  void SetBlendState(CommandList& cmdList, const BlendState& blending);

  void SetDepthStencil(CommandList& cmdList, const DepthStencilStateDescription& ds);
}
