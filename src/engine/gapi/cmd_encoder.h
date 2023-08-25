#pragma once

#include "resources.h"

namespace gapi
{
  class CmdEncoder
  {
    public:
      auto operator=(const CmdEncoder& rvl) -> const CmdEncoder& = delete;

      virtual
      ~CmdEncoder(){}

      virtual 
      void beginRenderpass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) = 0;

      virtual
      void endRenderpass() = 0;

      virtual
      void setScissor(const Scissor) = 0;

      virtual
      auto getScissor() -> Scissor = 0;

      virtual
      void setViewport(const Viewport&) = 0;

      virtual
      auto getViewport() -> Viewport const = 0;

      virtual
      void draw(const uint32_t vertexCount, const uint32_t instanceCount,
                const uint32_t firstVertex, const uint32_t firstInstance) = 0;

      virtual
      void drawIndexed(const uint32_t indexCount, uint32_t instanceCount,
                       const uint32_t firstIndex, const uint32_t vertexOffset,
                       const uint32_t firstInstace) = 0;

      virtual
      void bindVertexBuffer(const BufferHandler buffer) = 0;

      virtual
      void bindIndexBuffer(const BufferHandler buffer) = 0;

      virtual
      void bindGraphicsPipeline(const GraphicsPipelineDescription& desc) = 0;

      virtual
      void bindComputePipeline(const ComputePipelineDescription& desc) = 0;

      //virtual void present() = 0;

      //virtual void pushConstants(const void* data, const size_t size, const ShaderStage stage) = 0;

      virtual
      void bindConstBuffer(const BufferHandler buffer, const size_t set, const size_t binding) = 0;

      virtual
      void bindTexture(const TextureHandle texture, const size_t set, const size_t binding, const size_t mip = 0) = 0;

      virtual
      void bindSampler(const SamplerHandler sampler, const size_t set, const size_t binding) = 0;

      virtual
      void bindBuffer(const BufferHandler buffer, const size_t set, const size_t binding) = 0;

      virtual
      void flush(Fence* signalFence = nullptr) = 0;

      virtual
      void transitTextureState(const TextureHandle texture,
                               const TextureState oldState, const TextureState newState,
                               const uint32_t firstMipLevel = 0, const uint32_t mipLevelsCount = ~(0),
                               const uint32_t firstArraySlice = 0, const uint32_t arraySliceCount = ~(0)) = 0;

      virtual
      void insertGlobalBufferBarrier(const BufferState old_state, const BufferState new_state) = 0;

      virtual
      void updateResources() = 0;

      virtual
      void writeBuffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags = 0) = 0;

      virtual
      void copyTextureToBuffer(const TextureHandle src, const BufferHandler dst) = 0;

      virtual
      void copyBufferToTexture(const TextureHandle texture, const void* src, const size_t size) = 0;

      virtual
      void copyBufferToTexture(const void* buffer_src, const size_t buffer_size,
                                       const TextureHandle texture,
                                       const BufferTextureCopy* copy_descs, const size_t copy_count) = 0;

      virtual
      void blitTexture(const TextureHandle src, const TextureHandle dst, const uint32_t regions_count, const TextureBlit* regions, const ImageFilter filter) = 0;

      virtual
      void beginMarkerRegion(const char* name) = 0;

      virtual
      void endMarkerRegion() = 0;

      virtual
      void dispatch(const uint32_t group_count_x, const uint32_t group_count_y, const uint32_t group_count_z) = 0;

      virtual
      auto getRenderSize() const -> float2 = 0;

      virtual
      void clearColorTexture(const TextureHandle, const TextureState current_state, const TextureState final_state,
                             const ClearColorValue&, const TextureSubresourceRange&) = 0;
  };
}
