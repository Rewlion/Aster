#include "commands.h"

#include <boost/functional/hash.hpp>

namespace gapi
{
  void begin_renderpass(CommandList& cmdList, const RenderTargets& renderTargets, TextureHandler depthStencil)
  {
    cmdList.push_back(BeginRenderPassCmd{
      .renderTargets = renderTargets,
      .depthStencil = depthStencil
    });
  }

  void bind_graphics_shaders(CommandList& cmdList, const ShaderStagesNames& stages)
  {
    cmdList.push_back(BindGraphicsShadersCmd{
      .shaderNames = stages,
    });
  }

  void draw(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t vertexCount,
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

  void draw_indexed(CommandList& cmdList, const PrimitiveTopology topology, const uint32_t indexCount, uint32_t instanceCount,
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

  void present(CommandList& cmdList)
  {
    cmdList.push_back(PresentSurfaceImageCmd{});
  }

  void push_constants(CommandList& cmdList, const void* data, const size_t size, const ShaderStage stage)
  {
    void* managedData = new char[size];
    std::memcpy(managedData, data, size);

    cmdList.push_back(PushConstantsCmd{
      .data = managedData,
      .size = size,
      .stage = stage
    });
  }

  void bind_vertex_buffer(CommandList& cmdList, const BufferHandler buffer)
  {
    cmdList.push_back(BindVertexBufferCmd{
      .buffer = buffer
    });
  }

  void bind_index_buffer(CommandList& cmdList, const BufferHandler buffer)
  {
    cmdList.push_back(BindIndexBufferCmd{
      .buffer = buffer
    });
  }

  void bind_texture(CommandList& cmdList, const TextureHandler texture, const size_t argument, const size_t binding)
  {
    cmdList.push_back(BindTextureCmd{
      .texture = texture,
      .argument = argument,
      .binding = binding
    });
  }

  void bind_sampler(CommandList& cmdList, const SamplerHandler sampler, const size_t argument, const size_t binding)
  {
    cmdList.push_back(BindSamplerCmd{
      .sampler = sampler,
      .argument = argument,
      .binding = binding
    });
  }

  void clear(CommandList& cmdList, const ClearState clearing)
  {
    cmdList.push_back(ClearCmd{
      .clearing = clearing
    });
  }

  void set_blend_state(CommandList& cmdList, const BlendState& blending)
  {
    cmdList.push_back(SetBlendStateCmd{
      .blending = blending
    });
  }

  void set_depth_stencil(CommandList& cmdList, const DepthStencilStateDescription& ds)
  {
    cmdList.push_back(SetDepthStencilStateCmd{
      .ds = ds
    });
  }
}
