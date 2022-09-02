#pragma once

namespace gapi
{
  class CmdEncoder
  {
    public:
      const CmdEncoder& operator=(const CmdEncoder& rvl) = delete;

      virtual ~CmdEncoder(){}

      virtual void beginRenderpass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) = 0;

      virtual void endRenderpass() = 0;

      virtual void draw(const uint32_t vertexCount, const uint32_t instanceCount,
                        const uint32_t firstVertex, const uint32_t firstInstance) = 0;

      virtual void drawIndexed(const uint32_t indexCount, uint32_t instanceCount,
                               const uint32_t firstIndex, const uint32_t vertexOffset,
                               const uint32_t firstInstace) = 0;

      virtual void bindVertexBuffer(const BufferHandler buffer) = 0;

      virtual void bindIndexBuffer(const BufferHandler buffer) = 0;

      virtual void bindGraphicsPipeline(const GraphicsPipelineDescription& desc) = 0;

      //virtual void present() = 0;

      //virtual void pushConstants(const void* data, const size_t size, const ShaderStage stage) = 0;

      virtual void bindConstBuffer(const BufferHandler buffer, const size_t set, const size_t binding) = 0;

      virtual void bindTexture(const TextureHandler texture, const size_t set, const size_t binding) = 0;

      virtual void bindSampler(const SamplerHandler sampler, const size_t set, const size_t binding) = 0;

      virtual void flush(Fence* signalFence = nullptr) = 0;

      virtual void present(Semaphore* wait_semaphore) = 0;

      virtual Semaphore* signalSemaphore() = 0;

      virtual void insertSemaphore(Semaphore* s) = 0;

      virtual void transitTextureState(const TextureHandler texture,
                                       const TextureState oldState, const TextureState newState,
                                       const uint32_t firstMipLevel = 0, const uint32_t mipLevelsCount = ~(0),
                                       const uint32_t firstArraySlice = 0, const uint32_t arraySliceCount = ~(0)) = 0;

      virtual void updateResources() = 0;

      virtual void writeBuffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags = 0) = 0;
      virtual void copyBufferToTexture(const TextureHandler texture, const void* src, const size_t size) = 0;

      virtual void blitTexture(const TextureHandler src, const TextureHandler dst, const uint32_t regions_count, const TextureBlit* regions, const ImageFilter filter) = 0;
  };
}
