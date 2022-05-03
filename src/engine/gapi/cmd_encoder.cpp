#include "cmd_encoder.h"

#include "gapi.h"

namespace gapi
{
  void CmdEncoder::beginRenderpass(const RenderTargets& renderTargets, RenderPassAttachment depthStencil)
  {
    m_Cmds.push_back(BeginRenderPassCmd{
      .renderTargets = renderTargets,
      .depthStencil = depthStencil
    });
  }

  void CmdEncoder::bindGraphicsShaders(const ShaderStagesNames& stages)
  {
    m_Cmds.push_back(BindGraphicsShadersCmd{
      .shaderNames = stages,
    });
  }

  void CmdEncoder::draw(const PrimitiveTopology topology, const uint32_t vertexCount,
               const uint32_t instanceCount, const uint32_t firstVertex, const uint32_t firstInstance)
  {
    m_Cmds.push_back(DrawCmd{
      .topology = topology,
      .vertexCount = vertexCount,
      .instanceCount = instanceCount,
      .firstVertex = firstVertex,
      .firstInstance = firstInstance
    });
  }

  void CmdEncoder::drawIndexed(const PrimitiveTopology topology, const uint32_t indexCount, uint32_t instanceCount,
                      const uint32_t firstIndex, const uint32_t vertexOffset, const uint32_t firstInstance)
  {
    m_Cmds.push_back(DrawIndexedCmd{
      .topology = topology,
      .indexCount = indexCount,
      .instanceCount = instanceCount,
      .firstIndex = firstIndex,
      .vertexOffset = vertexOffset,
      .firstInstance = firstInstance
    });
  }

  void CmdEncoder::present()
  {
    m_Cmds.push_back(PresentSurfaceImageCmd{});
  }

  void CmdEncoder::pushConstants(const void* data, const size_t size, const ShaderStage stage)
  {
    void* managedData = new char[size];
    std::memcpy(managedData, data, size);

    m_Cmds.push_back(PushConstantsCmd{
      .data = managedData,
      .size = size,
      .stage = stage
    });
  }

  void CmdEncoder::bindVertexBuffer(const BufferHandler buffer)
  {
    m_Cmds.push_back(BindVertexBufferCmd{
      .buffer = buffer
    });
  }

  void CmdEncoder::bindIndexBuffer(const BufferHandler buffer)
  {
    m_Cmds.push_back(BindIndexBufferCmd{
      .buffer = buffer
    });
  }

  void CmdEncoder::bindConstBuffer(const BufferHandler buffer, const size_t argument, const size_t binding)
  {
    m_Cmds.push_back(BindConstantBufferCmd{
      .buffer = buffer,
      .argument = argument,
      .binding = binding
    });
  }

  void CmdEncoder::bindTexture(const TextureHandler texture, const size_t argument, const size_t binding)
  {
    m_Cmds.push_back(BindTextureCmd{
      .texture = texture,
      .argument = argument,
      .binding = binding
    });
  }

  void CmdEncoder::bindSampler(const SamplerHandler sampler, const size_t argument, const size_t binding)
  {
    m_Cmds.push_back(BindSamplerCmd{
      .sampler = sampler,
      .argument = argument,
      .binding = binding
    });
  }

  void CmdEncoder::clear(const int clearing)
  {
    m_Cmds.push_back(ClearCmd{
      .clearing = clearing
    });
  }

  void CmdEncoder::setBlendState(const BlendState& blending)
  {
    m_Cmds.push_back(SetBlendStateCmd{
      .blending = blending
    });
  }

  void CmdEncoder::setDepthStencil(const DepthStencilStateDescription& ds)
  {
    m_Cmds.push_back(SetDepthStencilStateCmd{
      .ds = ds
    });
  }

  void CmdEncoder::flush()
  {
    gapi::submit_commands(std::move(m_Cmds));
    m_Cmds.clear();
  }
}