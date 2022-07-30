#include "cmd_encoder.h"
#include "device.h"
#include "frame_gc.h"
#include "gapi_to_vk.h"

#include <engine/gapi/vulkan/cache/renderpass_storage.h>
#include <engine/gapi/vulkan/cache/pipelines_storage.h>

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
    m_FrameGc.addDsetManager(std::move(m_DsetManager));
    m_FrameGc.addCmdPool(std::move(m_CmdPool));
  }

  void CmdEncoder::beginRenderpass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clear)
  {
    const vk::Extent2D min = getMinRenderSize(renderTargets, depthStencil);
    const vk::Rect2D renderArea = vk::Rect2D{{0,0}, min};

    m_RenderPassState.renderTargets = renderTargets;
    m_RenderPassState.depthStencil = depthStencil;

    m_RenderPassState.rp = m_RenderPassStorage.getRenderPass(renderTargets, depthStencil, clear);
    m_RenderPassState.fb = getFramebuffer(renderArea.extent, renderTargets, depthStencil);

    std::array<uint32_t,4> clearColor{0,0,0,0};
    vk::ClearValue clearValue;
    clearValue.color = clearColor;

    vk::ClearValue clearValues[MAX_RENDER_TARGETS+1] {clearValue};
    clearValues[renderTargets.getSize()] = vk::ClearDepthStencilValue(1.0, 0);

    auto rpBeginInfo = vk::RenderPassBeginInfo();
    rpBeginInfo.renderPass = m_RenderPassState.rp;
    rpBeginInfo.framebuffer = m_RenderPassState.fb;
    rpBeginInfo.pClearValues = clearValues;
    rpBeginInfo.clearValueCount = std::size(clearValues);
    rpBeginInfo.renderArea = renderArea;

    insureActiveCmd();
    m_CmdBuf.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    m_GraphicsPipelineState.viewport = renderArea.extent;

    m_GraphicsPipelineState.description.blendState.attachments.clear();
    for (const auto& rt: renderTargets)
      m_GraphicsPipelineState.description.blendState.attachments.push( {} );
  }

  vk::Extent2D CmdEncoder::getMinRenderSize(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil) const
  {
    vk::Extent2D min = {(uint32_t)~(0), (uint32_t)~(0)};
    for(const auto& rt: renderTargets)
    {
      if (rt.texture != TextureHandler::Invalid)
      {
        vk::Extent3D dim = m_Device.getImageDim(rt.texture);
        min.width  = dim.width  < min.width  ? dim.width  : min.width;
        min.height = dim.height < min.height ? dim.height : min.height;
      }
    }

    if ((min != vk::Extent2D{0,0}) && (depthStencil.texture != TextureHandler::Invalid))
    {
      vk::Extent3D dim = m_Device.getImageDim(depthStencil.texture);
      min.width  = dim.width  < min.width  ? dim.width  : min.width;
      min.height = dim.height < min.height ? dim.height : min.height;
    }

    return min;
  }

  vk::RenderPass CmdEncoder::getRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing)
  {
    return m_RenderPassStorage.getRenderPass(renderTargets, depthStencil, clearing);
  }

  vk::Framebuffer CmdEncoder::getFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil)
  {
    vk::UniqueFramebuffer fb = createFramebuffer(renderArea, renderTargets, depthStencil);
    const auto framebuffer = fb.get();
    m_FrameGc.addFramebuffer(std::move(fb));

    return framebuffer;
  }

  vk::UniqueFramebuffer CmdEncoder::createFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil)
  {
    Utils::FixedStack<vk::ImageView, MAX_RENDER_TARGETS + 1> attachments;
    size_t attachmentsCount = 0;

    for(const auto& rt: renderTargets)
      attachments.push(m_Device.getImageView(rt.texture));

    if (depthStencil.texture != TextureHandler::Invalid)
      attachments.push(m_Device.getImageView(depthStencil.texture));

    auto fbCi = vk::FramebufferCreateInfo();
    fbCi.renderPass = m_RenderPassState.rp;
    fbCi.attachmentCount = attachments.getSize();
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
    m_FrameGc.addCmdBuffer(std::move(cmdBuf));

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
    m_CmdBuf.bindVertexBuffers(0, 1, &buffer.buffer.get(), &offsets);
  }

  void CmdEncoder::bindIndexBuffer(const BufferHandler handle)
  {
    insureActiveCmd();
    const Buffer& buffer = m_Device.getBuffer(handle);
    m_CmdBuf.bindIndexBuffer(buffer.buffer.get(), 0, vk::IndexType::eUint32);
  }

  void CmdEncoder::bindGraphicsPipeline(const GraphicsPipelineDescription& desc)
  {
    insureActiveCmd();

    const size_t bindedRtCount = m_RenderPassState.renderTargets.getSize();
    const size_t blendingRtCount = desc.blendState.attachments.getSize();

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

    m_GraphicsPipelineState.layout = m_PipelinesStorage.getPipelineLayout(desc.shaders);
    m_DsetManager.setPipelineLayout(m_GraphicsPipelineState.layout);
    m_CmdBuf.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

    vk::Viewport vp;
    vp.x = 0;
    vp.y = 0;
    vp.width =  (float)m_GraphicsPipelineState.viewport.width;
    vp.height = (float)m_GraphicsPipelineState.viewport.height;
    vp.minDepth = 0;
    vp.maxDepth = 1;
    m_CmdBuf.setViewport(0, 1, &vp);

    vk::Rect2D sc {{0,0}, m_GraphicsPipelineState.viewport};
    m_CmdBuf.setScissor(0, 1, &sc);
  }

  void CmdEncoder::flush(Fence* signalFence)
  {
    ASSERT(m_RenderPassState.rp == vk::RenderPass{});
    ASSERT(m_CmdBuf != vk::CommandBuffer{});
    m_CmdBuf.end();

    vk::Fence fence;
    if (!signalFence)
    {
      vk::UniqueFence f = m_Device.getDevice().createFenceUnique(vk::FenceCreateInfo{});
      fence = f.get();
      m_FrameGc.addWaitFence(std::move(f));
    }
    else
      fence = reinterpret_cast<VulkanFence*>(signalFence)->fence.get();

    m_Device.submitGraphicsCmds(&m_CmdBuf, 1,
      m_QueuedWaitSemaphores.data(), m_QueuedWaitSemaphores.size(),
      m_QueuedSignalSemaphores.data(), m_QueuedSignalSemaphores.size(),
      fence);

    m_QueuedWaitSemaphores.clear();
    m_QueuedSignalSemaphores.clear();
    m_CmdBuf = vk::CommandBuffer{};
  }

  Semaphore* CmdEncoder::signalSemaphore()
  {
    VulkanSemaphore* s = new VulkanSemaphore();
    s->semaphore = m_Device.createSemaphore();
    m_QueuedSignalSemaphores.push_back(s->semaphore.get());
    return s;
  }

  void CmdEncoder::insertSemaphore(Semaphore* s)
  {
    VulkanSemaphore* sem = reinterpret_cast<VulkanSemaphore*>(s);
    m_QueuedWaitSemaphores.push_back(sem->semaphore.get());
    m_FrameGc.addSemaphores(std::move(sem->semaphore));
  }

  void CmdEncoder::bindConstBuffer(const BufferHandler h, const size_t set, const size_t binding)
  {
    if (h != BufferHandler::Invalid)
    {
      const Buffer& buffer = m_Device.getBuffer(h);
      m_DsetManager.setUniformBuffer(buffer.buffer.get(), set, binding, buffer.getConstOffset());
    }
  }

  void CmdEncoder::bindTexture(const TextureHandler texture, const size_t set, const size_t binding)
  {
    if (texture != TextureHandler::Invalid)
    {
      vk::ImageView imgView = m_Device.getImageView(texture);
      m_DsetManager.setImage(imgView, set, binding);
    }
  }

  void CmdEncoder::bindSampler(const SamplerHandler h, const size_t set, const size_t binding)
  {
    if (h != SamplerHandler::Invalid)
    {
      vk::Sampler sampler = m_Device.getSampler(h);
      m_DsetManager.setSampler(sampler, set, binding);
    }
  }

  void CmdEncoder::transitTextureState(const TextureHandler texture,
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

  void CmdEncoder::updateResources()
  {
    insureActiveCmd();
    m_DsetManager.updateDescriptorSets(m_CmdBuf);
  }
}
