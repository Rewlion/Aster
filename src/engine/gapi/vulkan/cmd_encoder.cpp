#include "cmd_encoder.h"
#include "device.h"
#include "frame_gc.h"
#include "gapi_to_vk.h"
#include "tracy.h"

#include <engine/gapi/vulkan/cache/renderpass_storage.h>
#include <engine/gapi/vulkan/cache/pipelines_storage.h>

#undef MemoryBarrier //wtf ? winnt


namespace gapi::vulkan
{
  CmdEncoder::CmdEncoder(Device& device, FrameGarbageCollector& frameGc,
                         RenderPassStorage& rpStorage, PipelinesStorage& psStorage,
                         vk::UniqueCommandPool&& cmdPool)
    : m_Device(device)
    , m_FrameGc(frameGc)
    , m_RenderPassStorage(rpStorage)
    , m_PipelinesStorage(psStorage)
    , m_DsetManager(device)
    , m_CmdPool(std::move(cmdPool))
  {
  }

  CmdEncoder::~CmdEncoder()
  {
    m_FrameGc.add(std::move(m_DsetManager));
    m_FrameGc.add(std::move(m_CmdPool));
  }

  void CmdEncoder::beginRenderpass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    const vk::Extent2D min = getMinRenderSize(renderTargets, depthStencil);
    const vk::Rect2D renderArea = vk::Rect2D{{0,0}, min};
    m_RenderSize = float2{min.width, min.height};

    m_RenderPassState.renderTargets = renderTargets;
    m_RenderPassState.depthStencil = depthStencil;

    m_RenderPassState.rp = m_RenderPassStorage.getRenderPass(renderTargets, depthStencil);
    m_RenderPassState.fb = getFramebuffer(renderArea.extent, renderTargets, depthStencil);

    vk::ClearValue clearValues[MAX_RENDER_TARGETS+1];
    const size_t rtCount = renderTargets.count();
    for (size_t i = 0; i < rtCount ; ++i)
      clearValues[i].color.setUint32(renderTargets.get(i).clearColor.uint32);
    clearValues[renderTargets.count()] = vk::ClearDepthStencilValue(0.0, 0);

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = m_RenderPassState.rp;
    rpBeginInfo.framebuffer = m_RenderPassState.fb;
    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.clearValueCount = std::size(clearValues);
    rpBeginInfo.renderArea = renderArea;

    insureActiveCmd();
    m_CmdBuf.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    vk::Viewport vp;
    vp.x = 0;
    vp.y = 0;
    vp.width = renderArea.extent.width;
    vp.height = renderArea.extent.height;
    vp.minDepth = 0;
    vp.maxDepth = 1;

    m_GraphicsPipelineState.viewport = vp;
    m_GraphicsPipelineState.scissor = {{0,0}, renderArea.extent};

    m_GraphicsPipelineState.description.blendState.attachments.clear();
    for (const auto& rt: renderTargets)
      m_GraphicsPipelineState.description.blendState.attachments.push( {} );
  }

  vk::Extent2D CmdEncoder::getMinRenderSize(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil) const
  {
    vk::Extent2D min = {(uint32_t)~(0), (uint32_t)~(0)};
    for(const auto& rt: renderTargets)
    {
      if (rt.texture != TextureHandle::Invalid)
      {
        vk::Extent3D dim = m_Device.getImageDim(rt.texture);
        dim.width =  std::max((dim.width  >> rt.mipLevel), 1u);
        dim.height = std::max((dim.height >> rt.mipLevel), 1u);
        dim.depth =  std::max((dim.depth  >> rt.mipLevel), 1u);

        min.width  = dim.width  < min.width  ? dim.width  : min.width;
        min.height = dim.height < min.height ? dim.height : min.height;
      }
    }

    if ((min != vk::Extent2D{0,0}) && (depthStencil.texture != TextureHandle::Invalid))
    {
      vk::Extent3D dim = m_Device.getImageDim(depthStencil.texture);
      min.width  = dim.width  < min.width  ? dim.width  : min.width;
      min.height = dim.height < min.height ? dim.height : min.height;
    }

    return min;
  }

  vk::RenderPass CmdEncoder::getRenderPass(const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    return m_RenderPassStorage.getRenderPass(renderTargets, depthStencil);
  }

  vk::Framebuffer CmdEncoder::getFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    vk::UniqueFramebuffer fb = createFramebuffer(renderArea, renderTargets, depthStencil);
    const auto framebuffer = fb.get();
    m_FrameGc.add(std::move(fb));

    return framebuffer;
  }

  vk::UniqueFramebuffer CmdEncoder::createFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassDepthStencilAttachment& depthStencil)
  {
    Utils::FixedStack<vk::ImageView, MAX_RENDER_TARGETS + 1> attachments;
    size_t attachmentsCount = 0;

    for(const auto& rt: renderTargets)
      attachments.push(m_Device.getImageView(rt.texture, false, rt.mipLevel));

    if (depthStencil.texture != TextureHandle::Invalid)
      attachments.push(m_Device.getImageView(depthStencil.texture));

    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = m_RenderPassState.rp;
    fbCi.attachmentCount = attachments.count();
    fbCi.pAttachments = attachments.getData();
    fbCi.setWidth(renderArea.width);
    fbCi.setHeight(renderArea.height);
    fbCi.layers = 1;

    auto res = m_Device.getDevice().createFramebufferUnique(fbCi);
    VK_CHECK_RES(res);

    return std::move(res.value);
  }

  void CmdEncoder::insureActiveCmd()
  {
    if (m_CmdBuf == vk::CommandBuffer{})
      m_CmdBuf = allocateCmdBuffer();
  }

  vk::CommandBuffer CmdEncoder::allocateCmdBuffer()
  {
    const auto allocInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(m_CmdPool.get())
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

    auto res = m_Device.getDevice().allocateCommandBuffersUnique(allocInfo);
    VK_CHECK_RES(res);

    auto cmdBuf = std::move(res.value[0]);

    VK_CHECK(cmdBuf->begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    ));

    const auto ret = cmdBuf.get();
    m_FrameGc.add(std::move(cmdBuf));

    return ret;
  }

  void CmdEncoder::endRenderpass()
  {
    if (m_RenderPassState.rp != vk::RenderPass{})
    {
      insureActiveCmd();
      m_CmdBuf.endRenderPass();
      m_RenderPassState.rp = vk::RenderPass{};
    }
  }

  void CmdEncoder::setScissor(const Scissor sc)
  {
    m_GraphicsPipelineState.scissor = vk::Rect2D{ {sc.offset.x, sc.offset.y}, {sc.size.x, sc.size.y} };
    if (m_RenderPassState.rp != vk::RenderPass{})
      m_CmdBuf.setScissor(0, 1, &m_GraphicsPipelineState.scissor);
  }

  Scissor CmdEncoder::getScissor()
  {
    return Scissor{
      .offset = {m_GraphicsPipelineState.scissor.offset.x, m_GraphicsPipelineState.scissor.offset.y},
      .size = {m_GraphicsPipelineState.scissor.extent.width, m_GraphicsPipelineState.scissor.extent.height}
    };
  }

  void CmdEncoder::setViewport(const Viewport& vp)
  {
    vk::Viewport old = m_GraphicsPipelineState.viewport;

    m_GraphicsPipelineState.viewport.x = vp.pos.x;
    m_GraphicsPipelineState.viewport.y = vp.pos.y;
    m_GraphicsPipelineState.viewport.width = vp.size.x;
    m_GraphicsPipelineState.viewport.height = vp.size.y;
    m_GraphicsPipelineState.viewport.minDepth = vp.minDepth;
    m_GraphicsPipelineState.viewport.maxDepth = vp.maxDepth;

    if (old != m_GraphicsPipelineState.viewport)
      m_CmdBuf.setViewport(0, 1, &m_GraphicsPipelineState.viewport);
  }

  auto CmdEncoder::getViewport() -> Viewport const
  {
    return {
      .pos = {m_GraphicsPipelineState.viewport.x, m_GraphicsPipelineState.viewport.y},
      .size = {m_GraphicsPipelineState.viewport.width, m_GraphicsPipelineState.viewport.height},
      .minDepth = m_GraphicsPipelineState.viewport.minDepth,
      .maxDepth = m_GraphicsPipelineState.viewport.maxDepth
    };
  }

  void CmdEncoder::draw(const uint32_t vertexCount, const uint32_t instanceCount,
                        const uint32_t firstVertex, const uint32_t firstInstance)
  {
    insureActiveCmd();
    m_CmdBuf.draw(vertexCount, instanceCount, firstVertex, firstInstance);
  }

  void CmdEncoder::drawIndexed(const uint32_t indexCount, uint32_t instanceCount,
                               const uint32_t firstIndex, const uint32_t vertexOffset,
                               const uint32_t firstInstance)
  {
    insureActiveCmd();
    m_CmdBuf.drawIndexed(indexCount, instanceCount,
                         firstIndex, vertexOffset, firstInstance);
  }

  void CmdEncoder::bindVertexBuffer(const BufferHandler handle)
  {
    insureActiveCmd();
    const Buffer& buffer = m_Device.getBuffer(handle);
    vk::DeviceSize offsets = {0};
    m_CmdBuf.bindVertexBuffers(0, 1, &buffer.buffer, &offsets);
  }

  void CmdEncoder::bindIndexBuffer(const BufferHandler handle)
  {
    insureActiveCmd();
    const Buffer& buffer = m_Device.getBuffer(handle);
    m_CmdBuf.bindIndexBuffer(buffer.buffer, 0, vk::IndexType::eUint32);
  }

  void CmdEncoder::bindGraphicsPipeline(const GraphicsPipelineDescription& desc)
  {
    insureActiveCmd();

    const size_t bindedRtCount = m_RenderPassState.renderTargets.count();
    const size_t blendingRtCount = desc.blendState.attachments.count();

    vk::Pipeline pipeline;
    if (blendingRtCount >= bindedRtCount)
    {
      pipeline = m_PipelinesStorage.getPipeline(desc, m_RenderPassState.rp, 0);
    }
    else
    {
      GraphicsPipelineDescription pipeDesc = desc;
      const size_t missingBlendings = bindedRtCount - blendingRtCount;
      for (size_t i = 0; i < missingBlendings; ++i)
        pipeDesc.blendState.attachments.push({});

      pipeline = m_PipelinesStorage.getPipeline(pipeDesc, m_RenderPassState.rp, 0);
    }

    const PipelineLayout& layout = m_PipelinesStorage.getPipelineLayout(desc.layout);
    m_DsetManager.setPipelineLayout(&layout, vk::PipelineBindPoint::eGraphics);
    m_CmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

    vk::Viewport vp;
    vp.x = m_GraphicsPipelineState.viewport.x;
    vp.y = m_GraphicsPipelineState.viewport.height + m_GraphicsPipelineState.viewport.y;
    vp.width = m_GraphicsPipelineState.viewport.width;
    vp.height = -m_GraphicsPipelineState.viewport.height;
    vp.minDepth = m_GraphicsPipelineState.viewport.minDepth;
    vp.maxDepth = m_GraphicsPipelineState.viewport.maxDepth;

    m_CmdBuf.setViewport(0, 1, &vp);
    m_CmdBuf.setScissor(0, 1, &m_GraphicsPipelineState.scissor);
  }

  void CmdEncoder::bindComputePipeline(const ComputePipelineDescription& desc)
  {
    insureActiveCmd();

    vk::Pipeline pipeline = m_PipelinesStorage.getComputePipeline(desc);
    const PipelineLayout& layout = m_PipelinesStorage.getPipelineLayout(desc.layout);

    m_DsetManager.setPipelineLayout(&layout, vk::PipelineBindPoint::eCompute);
    m_CmdBuf.bindPipeline(vk::PipelineBindPoint::eCompute, pipeline);
  }

  void CmdEncoder::flush(Fence* signalFence)
  {
    insureActiveCmd();
    ASSERT(m_RenderPassState.rp == vk::RenderPass{});
    ASSERT(m_CmdBuf != vk::CommandBuffer{});
    
  #ifdef TRACY_ENABLE
    tracy_collect_time_stamps(m_CmdBuf);
  #endif
  
    VK_CHECK(m_CmdBuf.end());

    vk::Fence fence;
    if (!signalFence)
    {
      auto f = m_Device.getDevice().createFenceUnique(vk::FenceCreateInfo{});
      VK_CHECK_RES(f);
      VK_CHECK(m_Device.getDevice().resetFences(1, &f.value.get()));

      fence = f.value.get();
      m_FrameGc.addWaitFence(std::move(f.value));
    }
    else
      fence = reinterpret_cast<VulkanFence*>(signalFence)->fence.get();
    
    m_Device.submitGraphicsCmd(m_CmdBuf, fence);

    m_CmdBuf = vk::CommandBuffer{};
  }

  void CmdEncoder::bindConstBuffer(const BufferHandler h, const size_t set, const size_t binding)
  {
    m_DsetManager.setUniformBuffer(h, set, binding);
  }

  void CmdEncoder::bindTexture(const TextureHandle texture, const size_t set, const size_t binding, const size_t mip)
  {
    m_DsetManager.setImage(texture, set, binding, mip);
  }

  void CmdEncoder::bindSampler(const SamplerHandler h, const size_t set, const size_t binding)
  {
    if (h != SamplerHandler::Invalid)
    {
      vk::Sampler sampler = m_Device.getSampler(h);
      m_DsetManager.setSampler(sampler, set, binding);
    }
  }

  void CmdEncoder::bindBuffer(const BufferHandler h, const size_t set, const size_t binding)
  {
    m_DsetManager.setUniformBuffer(h, set, binding);
  }

  void CmdEncoder::transitTextureState(const TextureHandle texture,
                                       const TextureState oldState, const TextureState newState,
                                       const uint32_t firstMipLevel, const uint32_t mipLevelsCount,
                                       const uint32_t firstArraySlice, const uint32_t arraySliceCount)
  {
    ASSERT(m_RenderPassState.rp == vk::RenderPass{});
    insureActiveCmd();

    const vk::Format format = m_Device.getTextureFormat(texture);

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = firstMipLevel;
    subresourceRange.levelCount = mipLevelsCount;
    subresourceRange.baseArrayLayer = firstArraySlice;
    subresourceRange.layerCount = arraySliceCount;
    subresourceRange.aspectMask = get_image_aspect_flags(format);

    vk::ImageMemoryBarrier layoutBarrier;
    layoutBarrier.image = m_Device.getImage(texture);
    layoutBarrier.oldLayout = get_image_layout_for_texture_state(oldState);
    layoutBarrier.newLayout = get_image_layout_for_texture_state(newState);
    layoutBarrier.subresourceRange = subresourceRange;
    layoutBarrier.srcAccessMask = get_image_access_flags(oldState);
    layoutBarrier.dstAccessMask = get_image_access_flags(newState);

    m_CmdBuf.pipelineBarrier(
      get_pipeline_stage_for_texture_state(oldState),
      get_pipeline_stage_for_texture_state(newState),
      vk::DependencyFlags{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier
    );
  }

  void CmdEncoder::insertGlobalBufferBarrier(const BufferState old_state, const BufferState new_state)
  {
    ASSERT(m_RenderPassState.rp == vk::RenderPass{});
    insureActiveCmd();

    vk::MemoryBarrier memBarrier;
    memBarrier.srcAccessMask = get_access_flag_for_buffer_state(old_state);
    memBarrier.dstAccessMask = get_access_flag_for_buffer_state(new_state);

    m_CmdBuf.pipelineBarrier(
      get_pipeline_stage_for_buffer_state(old_state),
      get_pipeline_stage_for_buffer_state(new_state),
      vk::DependencyFlags{},
      1, &memBarrier,
      0, nullptr,
      0, nullptr
    );
  }

  void CmdEncoder::updateResources()
  {
    insureActiveCmd();
    m_DsetManager.updateDescriptorSets(m_CmdBuf);
  }

  void CmdEncoder::writeBuffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size, const int flags)
  {
    insureActiveCmd();
    Buffer* b = m_Device.getAllocatedBuffer(buffer);

    if ( !(b->usage & BF_CpuVisible | b->usage & BF_GpuVisible) )
      ASSERT(!"writeBuffer dst buffer don't have memory set");

    if (flags & WR_DISCARD)
      m_Device.discardBuffer(*b);

    Buffer staging = m_Device.allocateStagingBuffer(src, size);

    vk::BufferCopy region;
    region.size = size;
    region.srcOffset = 0;
    region.dstOffset = offset;

    m_CmdBuf.copyBuffer(staging.buffer, b->buffer, 1, &region);
    m_FrameGc.addBuffer(std::move(staging));
  }

  void CmdEncoder::copyTextureToBuffer(const TextureHandle src, const BufferHandler dst)
  {
    insureActiveCmd();
    Texture& t = m_Device.getAllocatedTexture(src);
    Buffer& b = *m_Device.getAllocatedBuffer(dst);

    vk::BufferImageCopy copyDesc;
    copyDesc.bufferOffset = 0;
    copyDesc.bufferRowLength = 0;
    copyDesc.bufferImageHeight = 0;
    copyDesc.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    copyDesc.imageSubresource.mipLevel = 0;
    copyDesc.imageSubresource.baseArrayLayer = 0;
    copyDesc.imageSubresource.layerCount = 1;
    copyDesc.imageOffset = vk::Offset3D{0,0,0};
    copyDesc.imageExtent = vk::Extent3D{(uint32_t)t.size.x, (uint32_t)t.size.y, 1};

    m_CmdBuf.copyImageToBuffer(t.img, vk::ImageLayout::eTransferSrcOptimal, b.buffer, 1, &copyDesc);
  }

  void CmdEncoder::copyBufferToTexture(const TextureHandle texture, const void* src, const size_t size)
  {
    insureActiveCmd();
    Texture& t = m_Device.getAllocatedTexture(texture);
    Buffer staging = m_Device.allocateStagingBuffer(src, size);

    vk::BufferImageCopy copyDesc;
    copyDesc.bufferOffset = 0;
    copyDesc.bufferRowLength = 0;
    copyDesc.bufferImageHeight = 0;
    copyDesc.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    copyDesc.imageSubresource.mipLevel = 0;
    copyDesc.imageSubresource.baseArrayLayer = 0;
    copyDesc.imageSubresource.layerCount = 1;
    copyDesc.imageOffset = vk::Offset3D{0,0,0};
    copyDesc.imageExtent = vk::Extent3D{(uint32_t)t.size.x, (uint32_t)t.size.y, 1};

    m_CmdBuf.copyBufferToImage(staging.buffer, t.img,
                               vk::ImageLayout::eTransferDstOptimal, 1, &copyDesc);
    m_FrameGc.addBuffer(std::move(staging));
  }

  void CmdEncoder::copyBufferToTexture(const void* buffer_src, const size_t buffer_size,
                                       const TextureHandle texture,
                                       const BufferTextureCopy* copy_descs, const size_t copy_count)
  {
    insureActiveCmd();
    Texture& t = m_Device.getAllocatedTexture(texture);
    Buffer staging = m_Device.allocateStagingBuffer(buffer_src, buffer_size);

    eastl::vector<vk::BufferImageCopy> copies;
    copies.reserve(copy_count);

    for (size_t i = 0; i < copy_count; ++i)
    {
      vk::BufferImageCopy copyDesc = get_buffer_img_copy(copy_descs[i]);
      copies.push_back(copyDesc);
    }

    m_CmdBuf.copyBufferToImage(staging.buffer, t.img,
      vk::ImageLayout::eTransferDstOptimal, copy_count, copies.data());

    m_FrameGc.addBuffer(std::move(staging));
  }

  void CmdEncoder::blitTexture(const TextureHandle src, const TextureHandle dst, const uint32_t regions_count, const TextureBlit* regions, const ImageFilter filter)
  {
    ASSERT(regions_count > 0);
    insureActiveCmd();

    const vk::Image srcImg = m_Device.getImage(src);
    const vk::Image dstImg = m_Device.getImage(dst);

    eastl::vector<vk::ImageBlit> blits;
    blits.reserve(regions_count);

    for (size_t i = 0; i < regions_count; ++i)
    {
      const TextureBlit& r = regions[i];
      blits.push_back(get_image_blit(r));
    }

    m_CmdBuf.blitImage(srcImg, vk::ImageLayout::eTransferSrcOptimal, dstImg, vk::ImageLayout::eTransferDstOptimal, regions_count, blits.data(), get_filter(filter));
  }

  void CmdEncoder::beginMarkerRegion(const char* name)
  {
  #if CFG_DEBUG_UTILS
    if (m_Device.checkCapability(Device::CapabilitiesBits::DebugMarks))
    {
      insureActiveCmd();
      vk::DebugUtilsLabelEXT label;
      label.pLabelName = name;
      m_CmdBuf.beginDebugUtilsLabelEXT(label);
    }
  #endif
  }

  void CmdEncoder::endMarkerRegion()
  {
  #if CFG_DEBUG_UTILS
    if (m_Device.checkCapability(Device::CapabilitiesBits::DebugMarks))
    {
      insureActiveCmd();
      m_CmdBuf.endDebugUtilsLabelEXT();
    }
  #endif
  }

#ifdef TRACY_ENABLE
  class VkGpuSectionProfileScope : public gapi::GpuSectionProfileScope
  {
    public:
      VkGpuSectionProfileScope(tracy::VkCtxScope&& scope)
        : m_Scope(std::move(scope))
      {
      }

      virtual
      ~VkGpuSectionProfileScope(){}

    private:
      tracy::VkCtxScope m_Scope;
      
  };
#endif

  auto CmdEncoder::beginScopedProfileSection(const size_t line, const char* file, const size_t file_len,
                                             const char* function, const size_t function_len, const char* name,
                                             const size_t name_len) -> GpuSectionProfileScope*
  {
  #ifdef TRACY_ENABLE
    insureActiveCmd();

    auto* ctx = reinterpret_cast<tracy::VkCtx*>(tracy_get_ctx());
    auto scope = tracy::VkCtxScope(ctx, line, file, file_len, function,
                                   function_len, name, name_len, m_CmdBuf, true);

    return new VkGpuSectionProfileScope{std::move(scope)};
  #endif

    return nullptr;
  }

  void CmdEncoder::dispatch(const uint32_t group_count_x, const uint32_t group_count_y, const uint32_t group_count_z)
  {
    insureActiveCmd();
    m_CmdBuf.dispatch(group_count_x, group_count_y, group_count_z);
  }

  void CmdEncoder::clearColorTexture(const TextureHandle tex, const TextureState current_state, const TextureState final_state,
                                     const ClearColorValue& color, const TextureSubresourceRange& range)
  {
    insureActiveCmd();

    const auto transitState = [&](const TextureState from, const TextureState to)
    {
      transitTextureState(tex, from, to,
                          range.baseMipLevel, range.levelCount,
                          range.baseArrayLayer, range.layerCount);
    };

    if (current_state != TextureState::TransferDst)
      transitState(current_state, TextureState::TransferDst);

    const vk::Image img = m_Device.getImage(tex);
    const vk::ImageLayout currentLayout = get_image_layout_for_texture_state(TextureState::TransferDst);
    const auto& vkColors = reinterpret_cast<const vk::ClearColorValue&>(color);
    
    vk::ImageSubresourceRange vkRange = get_image_subresource_range(range);

    m_CmdBuf.clearColorImage(img, currentLayout, &vkColors, 1, &vkRange);

    if (final_state != TextureState::TransferDst)
      transitState(TextureState::TransferDst, final_state);
  }

  void CmdEncoder::setCmdPromise(const gapi::CmdPromise& gapi_promise, const BufferState bf_state)
  {
    auto& promise = static_cast<const VulkanCmdPromise&>(gapi_promise);
    m_CmdBuf.setEvent(promise.event.get(),
      get_pipeline_stage_for_buffer_state(bf_state));
  }
}
