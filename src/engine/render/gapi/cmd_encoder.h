#pragma once

#include "commands.h"

namespace gapi
{
  class CmdEncoder
  {
    public:

      void beginRenderpass(const RenderTargets& renderTargets, TextureHandler depthStencil = TextureHandler::Invalid);

      void bindGraphicsShaders(const ShaderStagesNames& stages);

      void draw(const PrimitiveTopology topology, const uint32_t vertexCount,
                const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance);

      void drawIndexed(const PrimitiveTopology topology, const uint32_t indexCount, uint32_t instanceCount,
                        const uint32_t firstIndex, const uint32_t vertexOffset, const uint32_t firstInstace);

      void present();

      void pushConstants(const void* data, const size_t size, const ShaderStage stage);

      void bindVertexBuffer(const BufferHandler buffer);

      void bindIndexBuffer(const BufferHandler buffer);

      void bindConstBuffer(const BufferHandler buffer, const size_t argument, const size_t binding);

      void bindTexture(const TextureHandler texture, const size_t argument, const size_t binding);

      void bindSampler(const SamplerHandler sampler, const size_t argument, const size_t binding);

      void clear(const ClearState clearing);

      void setBlendState(const BlendState& blending);

      void setDepthStencil(const DepthStencilStateDescription& ds);

      void flush();

    private:
      CommandList m_Cmds;
  };
}
