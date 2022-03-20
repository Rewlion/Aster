#include "commands.h"

#include <boost/functional/hash.hpp>

namespace gapi
{
  void BeginRenderPass(CommandList& cmdList, const RenderTargets& renderTargets, TextureHandler depthStencil)
  {
    cmdList.push_back(BeginRenderPassCmd{
      .renderTargets = renderTargets,
      .depthStencil = depthStencil
    });
  }

  void BindGraphicsPipeline(CommandList& cmdList, const ShaderStagesNames& stages,
                            const DepthStencilStateDescription& depthStencilState)
  {
    cmdList.push_back(BindGraphicsPipelineCmd{
      .shaderNames = stages,
      .depthStencilState = depthStencilState
    });
  }

  void Draw(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t vertexCount,
               const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
  {
    cmdList.push_back(DrawCmd{
      .topology = topology,
      .vertexCount = vertexCount,
      .instanceCount = instanceCount,
      .firstVertex = firstVertex,
      .firstInstance = firstInstance
    });
  }

  void DrawIndexed(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t indexCount, uint32_t instanceCount,
                      const uint32_t firstIndex, const uint32_t vertexOffset, const uint32_t firstInstance)
  {
    cmdList.push_back(DrawIndexedCmd{
      .topology = topology,
      .indexCount = indexCount,
      .instanceCount = instanceCount,
      .firstIndex = firstIndex,
      .vertexOffset = vertexOffset,
      .firstInstance = firstInstance
    });
  }

  void PresentSurfaceImage(CommandList& cmdList)
  {
    cmdList.push_back(PresentSurfaceImageCmd{});
  }

  void PushConstants(CommandList& cmdList, const void* data, const size_t size, const ShaderStage stage)
  {
    cmdList.push_back(PushConstantsCmd{
      .data = data,
      .size = size,
      .stage = stage
    });
  }

  void BindVertexBuffer(CommandList& cmdList, const BufferHandler buffer)
  {
    cmdList.push_back(BindVertexBufferCmd{
      .buffer = buffer
    });
  }

  void BindIndexBuffer(CommandList& cmdList, const BufferHandler buffer)
  {
    cmdList.push_back(BindIndexBufferCmd{
      .buffer = buffer
    });
  }

  void BindTexture(CommandList& cmdList, const TextureHandler texture, const size_t argument, const size_t binding)
  {
    cmdList.push_back(BindTextureCmd{
      .texture = texture,
      .argument = argument,
      .binding = binding
    });
  }

  void BindSampler(CommandList& cmdList, const SamplerHandler sampler, const size_t argument, const size_t binding)
  {
    cmdList.push_back(BindSamplerCmd{
      .sampler = sampler,
      .argument = argument,
      .binding = binding
    });
  }

  void Clear(CommandList& cmdList, const ClearState clearing)
  {
    cmdList.push_back(ClearCmd{
      .clearing = clearing
    });
  }

  void SetBlendState(CommandList& cmdList, const BlendState& blending)
  {
    cmdList.push_back(SetBlendStateCmd{
      .blending = blending
    });
  }
}
