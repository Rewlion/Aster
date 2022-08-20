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
  };

  class CmdEncoder: public gapi::CmdEncoder
  {
    public:
      CmdEncoder(Device& device, FrameGarbageCollector& frameGc,
                 RenderPassStorage& rpStorage, PipelinesStorage& psStorage,
                 vk::UniqueCommandPool&& cmdPool);
      virtual ~CmdEncoder();
      virtual void beginRenderpass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) override;
      virtual void endRenderpass() override;
      virtual void draw(const uint32_t vertexCount, const uint32_t instanceCount,
                        const uint32_t firstVertex, const uint32_t firstInstance) override;
      virtual void drawIndexed(const uint32_t indexCount, uint32_t instanceCount,
                               const uint32_t firstIndex, const uint32_t vertexOffset,
                               const uint32_t firstInstace) override;
      virtual void bindVertexBuffer(const BufferHandler buffer) override;
      virtual void bindIndexBuffer(const BufferHandler buffer) override;
      virtual void bindGraphicsPipeline(const GraphicsPipelineDescription& desc) override;
      virtual void flush(Fence* signalFence = nullptr) override;
      virtual void present(Semaphore* wait_semaphore) override;
      virtual Semaphore* signalSemaphore() override;
      virtual void insertSemaphore(Semaphore* s) override;
      virtual void bindConstBuffer(const BufferHandler buffer, const size_t set, const size_t binding) override;
      virtual void bindTexture(const TextureHandler texture, const size_t set, const size_t binding) override;
      virtual void bindSampler(const SamplerHandler sampler, const size_t set, const size_t binding) override;
      virtual void transitTextureState(const TextureHandler texture,
                                       const TextureState oldState, const TextureState newState,
                                       const uint32_t firstMipLevel, const uint32_t mipLevelsCount,
                                       const uint32_t firstArraySlice, const uint32_t arraySliceCount) override;
      virtual void updateResources() override;
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

      eastl::vector<vk::Semaphore> m_QueuedWaitSemaphores;
      eastl::vector<vk::Semaphore> m_QueuedSignalSemaphores;

      vk::CommandBuffer m_CmdBuf;
      RenderPassState m_RenderPassState;
      GraphicsPipelineState m_GraphicsPipelineState;
  };
}
