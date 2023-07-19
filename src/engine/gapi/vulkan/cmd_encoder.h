#pragma once

#include "descriptors_set_manager.h"

#include <engine/gapi/cmd_encoder.h>

namespace gapi::vulkan
{
  class Device;
  class FrameGarbageCollector;
  class RenderPassStorage;
  class PipelinesStorage;
  class DescriptorsSetManager;

  struct RenderPassState
  {
    RenderTargets renderTargets;
    RenderPassDepthStencilAttachment depthStencil;

    vk::RenderPass rp;
    vk::Framebuffer fb;
  };

  struct GraphicsPipelineState
  {
    GraphicsPipelineDescription description;
    vk::Extent2D viewport;
    vk::Rect2D scissor;
  };

  class CmdEncoder: public gapi::CmdEncoder
  {
    public:
      CmdEncoder(Device& device, FrameGarbageCollector& frameGc,
                 RenderPassStorage& rpStorage, PipelinesStorage& psStorage,
                 vk::UniqueCommandPool&& cmdPool);
      ~CmdEncoder();

      void beginRenderpass(const RenderTargets& renderTargets,
                           const RenderPassDepthStencilAttachment& depthStencil) override;

      void endRenderpass() override;

      void setScissor(const Scissor) override;

      auto getScissor() -> Scissor override;

      void draw(const uint32_t vertexCount, const uint32_t instanceCount,
                        const uint32_t firstVertex, const uint32_t firstInstance) override;
      void drawIndexed(const uint32_t indexCount, uint32_t instanceCount,
                       const uint32_t firstIndex, const uint32_t vertexOffset,
                       const uint32_t firstInstace) override;

      void bindVertexBuffer(const BufferHandler buffer) override;

      void bindIndexBuffer(const BufferHandler buffer) override;

      void bindGraphicsPipeline(const GraphicsPipelineDescription& desc) override;

      void bindComputePipeline(const ComputePipelineDescription& desc) override;

      void flush(Fence* signalFence = nullptr) override;

      void bindConstBuffer(const BufferHandler buffer, const size_t set, const size_t binding) override;

      void bindTexture(const TextureHandle texture, const size_t set, const size_t binding) override;

      void bindSampler(const SamplerHandler sampler, const size_t set, const size_t binding) override;

      void transitTextureState(const TextureHandle texture,
                               const TextureState oldState, const TextureState newState,
                               const uint32_t firstMipLevel, const uint32_t mipLevelsCount,
                               const uint32_t firstArraySlice, const uint32_t arraySliceCount) override;

      void insertGlobalBufferBarrier(const BufferState old_state, const BufferState new_state) override;

      void updateResources() override;

      void writeBuffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags) override;

      void copyBufferToTexture(const TextureHandle texture, const void* src, const size_t size) override;

      void copyBufferToTexture(const void* buffer_src, const size_t buffer_size,
                               const TextureHandle texture,
                               const BufferTextureCopy* copy_descs, const size_t copy_count) override;

      void blitTexture(const TextureHandle src, const TextureHandle dst,
                       const uint32_t regions_count, const TextureBlit* regions,
                       const ImageFilter filter) override;

      void beginMarkerRegion(const char* name) override;

      void endMarkerRegion() override;

      void dispatch(const uint32_t group_count_x, const uint32_t group_count_y, const uint32_t group_count_z) override;
    private:
      vk::Extent2D getMinRenderSize(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) const;
      vk::RenderPass getRenderPass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil);
      vk::Framebuffer getFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil);
      vk::UniqueFramebuffer createFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil);

      void insureActiveCmd();
      vk::CommandBuffer allocateCmdBuffer();
    private:
      Device& m_Device;
      FrameGarbageCollector& m_FrameGc;
      RenderPassStorage& m_RenderPassStorage;
      PipelinesStorage& m_PipelinesStorage;
      DescriptorsSetManager m_DsetManager;
      vk::UniqueCommandPool m_CmdPool;

      vk::CommandBuffer m_CmdBuf;
      RenderPassState m_RenderPassState;
      GraphicsPipelineState m_GraphicsPipelineState;
  };
}
