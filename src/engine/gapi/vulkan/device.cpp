#include "device.h"
#include "frame_gc.h"
#include "gapi_to_vk.h"
#include "resources.h"
#include "result.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <cstring>

namespace gapi::vulkan
{
  Device::Device(CreateInfo&& ci, FrameGarbageCollector* frameGc)
    : m_FrameGc(frameGc)
    , m_Device(std::move(ci.device))
    , m_DeviceProperties(ci.deviceProperties)
    , m_QueueIndices(ci.queueIndices)
    , m_MemoryIndices(ci.memoryIndices)
  {
    vk::Instance& instance = ci.instance;

    m_GraphicsQueue = m_Device->getQueue(ci.queueIndices.graphics, 0);
    m_TransferQueue = m_Device->getQueue(ci.queueIndices.transfer, 0);

    const auto createPool = [this](const uint32_t index)
                            {
                              const auto cmdPoolCreateInfo = vk::CommandPoolCreateInfo()
                                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                .setQueueFamilyIndex(index);

                              auto pool = m_Device->createCommandPoolUnique(cmdPoolCreateInfo);
                              VK_CHECK_RES(pool);
                              return std::move(pool.value);
                            };

    m_GraphicsCmdPool = createPool(ci.queueIndices.graphics);
    m_TransferCmdPool = createPool(ci.queueIndices.transfer);

    vk::CommandBuffer swapchainInitCmdBuf = allocateGraphicsCmdBuffer();

    Swapchain::CreateInfo swpCi;
    swpCi.surface = ci.surface;
    swpCi.device = m_Device.get();
    swpCi.presentQueue = m_Device->getQueue(ci.queueIndices.present, 0);
    swpCi.surfaceCapabilities = ci.surfaceCapabilities;
    swpCi.surfaceFormats = ci.surfaceFormats;
    swpCi.surfacePresentModes = ci.surfacePresentModes;
    swpCi.swapchainImageExtent = ci.swapchainImageExtent;
    swpCi.initCmdBuf = &swapchainInitCmdBuf;
    m_Swapchain = Swapchain(swpCi);

    VK_CHECK(swapchainInitCmdBuf.end());
    auto fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    VK_CHECK_RES(fence);
    VK_CHECK(m_Device->resetFences(1, &fence.value.get()));

    submitGraphicsCmds(&swapchainInitCmdBuf, 1 , nullptr, 0, nullptr, 0, fence.value.get());
    VK_CHECK(m_Device->waitForFences(1, &fence.value.get(), true, ~0));
  }

  vk::CommandBuffer Device::allocateCmdBuffer(vk::CommandPool pool)
  {
    const auto allocInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

    auto r = m_Device->allocateCommandBuffersUnique(allocInfo);
    VK_CHECK_RES(r);
    auto cmdBuf = std::move(r.value[0]);

    VK_CHECK(cmdBuf->begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    ));

    const auto ret = cmdBuf.get();
    m_FrameGc->addCmdBuffer(std::move(cmdBuf));

    return ret;
  }

  vk::CommandBuffer Device::allocateGraphicsCmdBuffer()
  {
    return allocateCmdBuffer(*m_GraphicsCmdPool);
  }

  vk::CommandBuffer Device::allocateTransferCmdBuffer()
  {
    return allocateCmdBuffer(*m_TransferCmdPool);
  }

  vk::Format Device::getTextureFormat(const TextureHandle handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.getSurfaceFormat();

    if (h.as.typed.type == (uint64_t)TextureType::Allocated)
      return getAllocatedTexture(handler).format;

    ASSERT(!"UNSUPPORTED");
    return vk::Format::eUndefined;
  }

  vk::ImageView Device::getImageView(const TextureHandle handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.getSurfaceImageView();

    if (h.as.typed.type == (uint64_t)TextureType::Allocated)
    {
      ASSERT(m_AllocatedTextures.contains(h.as.typed.id));
      return m_AllocatedTextures.get(h.as.typed.id).view.get();
    }

    ASSERT(!"UNSUPPORTED");
    return vk::ImageView{};
  }

  vk::Image Device::getImage(const TextureHandle handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::Allocated)
     return m_AllocatedTextures.get(h.as.typed.id).img.get();

    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.getSurfaceImage();

    ASSERT(!"unsupported image type");
    return {};
  }

  vk::Extent3D Device::getImageDim(const TextureHandle handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
    {
      auto dim = m_Swapchain.getSurfaceExtent();
      return {dim.width, dim.height, 0};
    }

    if (h.as.typed.type == (uint64_t)TextureType::Allocated)
    {
      ASSERT(m_AllocatedTextures.contains(h.as.typed.id));
      const auto dim = m_AllocatedTextures.get(h.as.typed.id).size;
      return {(uint32_t)dim.x, (uint32_t)dim.y, (uint32_t)dim.z};
    }

    ASSERT(!"UNSUPPORTED");
    return {0,0,0};
  }

  void Device::submitGraphicsCmds(vk::CommandBuffer* cmdBuf, const size_t count,
                                             const vk::Semaphore* waitSemaphores, const size_t waitSemaphoresCount,
                                             const vk::Semaphore* signalSemaphores, const size_t signalSemaphoresCount,
                                             const vk::Fence signalFence)
  {
    const vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::SubmitInfo submit;
    submit.pCommandBuffers = cmdBuf;
    submit.commandBufferCount = count;
    submit.pWaitSemaphores = waitSemaphores;
    submit.waitSemaphoreCount = waitSemaphoresCount;
    submit.pWaitDstStageMask = &waitStage;
    submit.signalSemaphoreCount = signalSemaphoresCount;
    submit.pSignalSemaphores = signalSemaphores;
    VK_CHECK(m_GraphicsQueue.submit(submit, signalFence));
  }

  void Device::presentSurfaceImage(vk::Semaphore wait_semaphore)
  {
    m_Swapchain.present(wait_semaphore);
  }

  BufferHandler Device::allocateBuffer(const size_t size, const int usage)
  {
    Buffer buffer = allocateBufferInternal(size, usage);

    size_t id = (size_t)(~0);
    const bool allocated = m_AllocatedBuffers.add(std::move(buffer), id);

    ASSERT(allocated);
    return (BufferHandler)id;
  }

  void Device::freeBuffer(const BufferHandler buffer)
  {
    const size_t id = (size_t)buffer;
    ASSERT(m_AllocatedBuffers.contains(id));

    m_AllocatedBuffers.remove(id);
  }

  void* Device::mapBuffer(const BufferHandler buffer, const size_t offset, const size_t size, const int flags)
  {
    Buffer* b = getAllocatedBuffer(buffer);
    if (!(b->usage & BF_CpuVisible))
    {
      ASSERT(!"can't map not cpu visible buffer");
      return nullptr;
    }

    if (flags & WR_DISCARD)
      discardBuffer(*b);

    return mapBuffer(*b, offset, size);
  }

  void* Device::mapBuffer(const Buffer& buffer, const size_t offset, const size_t size)
  {
    void* mem = nullptr;

    VK_CHECK(m_Device->mapMemory(buffer.memory.get(), buffer.getAbsOffset(offset), size, vk::MemoryMapFlagBits{}, &mem));

    return mem;
  }

  void Device::unmapBuffer(const BufferHandler buffer)
  {
    Buffer* b = getAllocatedBuffer(buffer);
    if (!(b->usage & BF_CpuVisible))
    {
      ASSERT(!"can't unmap not cpu visible buffer");
      return;
    }

    unmapBuffer(*b);
  }

  void Device::unmapBuffer(const Buffer& buffer)
  {
    m_Device->unmapMemory(buffer.memory.get());
  }

  Buffer Device::allocateBufferInternal(const size_t size, const int usage)
  {
    Buffer b;

    b.usage = usage;
    b.blockSize = size;
    b.maxDiscards = get_buffer_discards_count(usage);

    vk::BufferCreateInfo bufferCi;
    bufferCi.usage = get_buffer_usage(usage);
    bufferCi.size = get_buffer_size(size, b.maxDiscards, usage, m_DeviceProperties.limits);
    bufferCi.sharingMode = vk::SharingMode::eExclusive;

    b.alignedBlockSize = bufferCi.size / b.maxDiscards;

    auto bufRes = m_Device->createBufferUnique(bufferCi);
    VK_CHECK_RES(bufRes);
    b.buffer = std::move(bufRes.value);
    const vk::MemoryRequirements memRec = m_Device->getBufferMemoryRequirements(b.buffer.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRec.size;
    allocInfo.memoryTypeIndex = get_memory_index(m_MemoryIndices, usage);

    auto memRes = m_Device->allocateMemoryUnique(allocInfo);
    VK_CHECK_RES(memRes);
    b.memory = std::move(memRes.value);

    VK_CHECK(m_Device->bindBufferMemory(b.buffer.get(), b.memory.get(), 0));

    return b;
  }

  Buffer Device::allocateStagingBuffer(const void* src, const size_t size)
  {
    Buffer stagingBuf = allocateBufferInternal(size, BF_CpuVisible);

    void* mappedMemory = mapBuffer(stagingBuf, 0, size);
    std::memcpy(mappedMemory, src, size);
    unmapBuffer(stagingBuf);

    return stagingBuf;
  }

  Buffer* Device::getAllocatedBuffer(const BufferHandler handler)
  {
    const size_t id = (size_t)handler;
    if (!m_AllocatedBuffers.contains(id))
    {
      logerror("vulkan: getAllocatedBuffer: buffer({}) not allocated", id);
      ASSERT(!"failed to get allocated buffer");
      return nullptr;
    }

    return &m_AllocatedBuffers.get(id);
  }

  void Device::discardBuffer(Buffer& buffer)
  {
    if (buffer.isFirstDiscard)
    {
      buffer.isFirstDiscard = false;
      return;
    }

    buffer.discards++;
    if (buffer.discards == buffer.maxDiscards)
    {
      m_FrameGc->addBuffer(std::move(buffer.buffer));
      m_FrameGc->addMemory(std::move(buffer.memory));

      buffer = allocateBufferInternal(buffer.blockSize, buffer.usage);
      buffer.isFirstDiscard = false;
    }
  }

  void Device::copyBuffersSync(const BufferHandler src, const size_t srcOffset,
                               const BufferHandler dst, const size_t dstOffset,
                               const size_t size)
  {
    const Buffer* srcBuf = getAllocatedBuffer(src);
    const Buffer* dstBuf = getAllocatedBuffer(dst);

    copyBuffersSync(srcBuf->buffer.get(), srcBuf->getAbsOffset(srcOffset),
                    dstBuf->buffer.get(), dstBuf->getAbsOffset(dstOffset), size);
  }

  void Device::copyBuffersSync(const vk::Buffer src, const size_t srcOffset,
                               const vk::Buffer dst, const size_t dstOffset,
                               const size_t size)
  {
    const auto cmdBuf = allocateTransferCmdBuffer();
    vk::BufferCopy region;
    region.size = size;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    cmdBuf.copyBuffer(src, dst, 1, &region);

    VK_CHECK(cmdBuf.end());

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    auto fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    VK_CHECK_RES(fence);
    VK_CHECK(m_Device->resetFences(1, &fence.value.get()));
    VK_CHECK(m_TransferQueue.submit(1, &submitInfo, fence.value.get()));
    VK_CHECK(m_Device->waitForFences(1, &fence.value.get(), true, ~0));
  }

  void Device::writeToStagingBuffer(const Buffer& buffer, const void* src, const size_t offset, const size_t size)
  {
    void* dst = mapBuffer(buffer, offset, size);
    std::memcpy(dst, src, size);
    unmapBuffer(buffer);
  }

  Buffer& Device::getBuffer(const BufferHandler buffer)
  {
    const size_t id = (size_t)buffer;
    return m_AllocatedBuffers.get(id);
  }

  const Buffer& Device::getBuffer(const BufferHandler buffer) const
  {
    return const_cast<Device*>(this)->getBuffer(buffer);
  }

  TextureHandle Device::allocateTexture(const TextureAllocationDescription& allocDesc)
  {
    Texture resource;
    resource.size = allocDesc.extent;

    vk::ImageCreateInfo ci{};

    ci.flags = get_image_create_flags(allocDesc.usage);
    ci.imageType = get_image_type(allocDesc.extent);
    ci.format = get_image_format(allocDesc.format);
    ci.extent = vk::Extent3D{(uint32_t)allocDesc.extent.x, (uint32_t)allocDesc.extent.y, (uint32_t)allocDesc.extent.z};
    ci.mipLevels = allocDesc.mipLevels;
    ci.arrayLayers = allocDesc.arrayLayers;
    ci.samples = get_image_sample_count(allocDesc.samplesPerPixel);
    ci.tiling = vk::ImageTiling::eOptimal;
    ci.usage = get_texture_usage(allocDesc.usage);
    ci.sharingMode = vk::SharingMode::eExclusive;

    uint32_t indices = m_QueueIndices.graphics;
    ci.pQueueFamilyIndices = &indices;
    ci.queueFamilyIndexCount = 1;

    ci.initialLayout = vk::ImageLayout::eUndefined;

    auto img = m_Device->createImageUnique(ci);
    VK_CHECK_RES(img);
    resource.img = std::move(img.value);
    resource.format = ci.format;

    const vk::MemoryRequirements memRec = m_Device->getImageMemoryRequirements(resource.img.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRec.size;
    allocInfo.memoryTypeIndex = m_MemoryIndices.deviceLocalMemory;

    auto memRes = m_Device->allocateMemoryUnique(allocInfo);
    VK_CHECK_RES(memRes);
    resource.memory = std::move(memRes.value);

    VK_CHECK(m_Device->bindImageMemory(resource.img.get(), resource.memory.get(), 0));

    vk::ImageSubresourceRange subresRange;
    subresRange.aspectMask = get_image_aspect_flags(ci.format);
    subresRange.baseArrayLayer = 0;
    subresRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresRange.baseMipLevel = 0;
    subresRange.levelCount = VK_REMAINING_MIP_LEVELS;
    vk::ImageViewCreateInfo viewCi;
    viewCi.image = resource.img.get();
    viewCi.viewType = get_image_view_type(ci.imageType, allocDesc.usage);
    viewCi.format = ci.format;
    viewCi.components = vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    viewCi.subresourceRange = subresRange;

    auto view = m_Device->createImageViewUnique(viewCi);
    VK_CHECK_RES(view);
    resource.view = std::move(view.value);

    size_t id = ~0;
    m_AllocatedTextures.add(std::move(resource), id);

    TextureHandlerInternal handler;
    handler.as.typed.id = id;
    handler.as.typed.type = (uint64_t)TextureType::Allocated;

    return handler;
  }

  void Device::freeTexture(const TextureHandle texture)
  {
    TextureHandlerInternal handler;
    handler.as.handler = uint64_t(texture);
    uint32_t textureId = handler.as.typed.id;

    ASSERT_FMT(handler.as.typed.type == (uint64_t)TextureType::Allocated, "only allocated textures can be freed");
    ASSERT(m_AllocatedTextures.contains(textureId));

    m_AllocatedTextures.remove(textureId);
  }

  SamplerHandler Device::allocateSampler(const SamplerAllocationDescription& allocDesc)
  {
    vk::SamplerCreateInfo samplerCi;
    samplerCi.magFilter = get_filter(allocDesc.magFilter);
    samplerCi.minFilter = get_filter(allocDesc.minFilter);
    samplerCi.mipmapMode = get_sampler_mimpmap_mode(allocDesc.mipmapMode);
    samplerCi.addressModeU = get_sampler_address_mode(allocDesc.addressModeU);
    samplerCi.addressModeV = get_sampler_address_mode(allocDesc.addressModeV);
    samplerCi.addressModeW = get_sampler_address_mode(allocDesc.addressModeW);
    samplerCi.mipLodBias = allocDesc.mipLodBias;
    samplerCi.anisotropyEnable = allocDesc.anisotropyEnable;
    samplerCi.maxAnisotropy = allocDesc.maxAnisotropy;
    samplerCi.compareEnable = allocDesc.compareEnable;
    samplerCi.compareOp = get_compare_op(allocDesc.compareOp);
    samplerCi.minLod = allocDesc.minLod;
    samplerCi.maxLod = allocDesc.maxLod;
    samplerCi.borderColor = get_border_color(allocDesc.borderColor);
    samplerCi.unnormalizedCoordinates = allocDesc.unnormalizedCoordinates;

    Sampler sampler;
    auto sRes = m_Device->createSamplerUnique(samplerCi);
    VK_CHECK_RES(sRes);
    sampler.sampler = std::move(sRes.value);

    size_t id = (size_t)(~0);
    const bool allocated = m_AllocatedSamplers.add(std::move(sampler), id);

    ASSERT(allocated);

    return SamplerHandler(id);
  }

  vk::Sampler Device::getSampler(const SamplerHandler sampler)
  {
    const size_t id = static_cast<size_t>(sampler);
    if (!m_AllocatedSamplers.contains(id))
    {
      ASSERT(!"Failed to get sampler: not allocated");
      return {};
    }

    return m_AllocatedSamplers.get(id).sampler.get();
  }

  vk::UniqueSemaphore Device::createSemaphore()
  {
    const auto semaphoreTypeCi = vk::SemaphoreTypeCreateInfo()
      .setInitialValue(0)
      .setSemaphoreType(vk::SemaphoreType::eBinary);

    const auto semaphoreCi = vk::SemaphoreCreateInfo()
      .setPNext(&semaphoreTypeCi);

    auto sem = m_Device->createSemaphoreUnique(semaphoreCi);
    VK_CHECK_RES(sem);
    return std::move(sem.value);
  }

  Texture& Device::getAllocatedTexture(const TextureHandle texture)
  {
    TextureHandlerInternal handler;
    handler.as.handler = uint64_t(texture);
    const uint32_t textureId = handler.as.typed.id;

    ASSERT(handler.as.typed.type == (uint64_t)TextureType::Allocated);
    ASSERT(m_AllocatedTextures.contains(textureId));

    return m_AllocatedTextures.get(textureId);
  }

  vk::UniqueCommandPool Device::allocateCmdPool()
  {
    vk::CommandPoolCreateInfo ci;
    ci.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    ci.queueFamilyIndex = m_QueueIndices.graphics;

    auto pool = m_Device->createCommandPoolUnique(ci);
    VK_CHECK_RES(pool);

    return std::move(pool.value);
  }

  VulkanFence* Device::allocateFence()
  {
    VulkanFence* f = new VulkanFence{};
    auto fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    VK_CHECK_RES(fence);

    f->fence = std::move(fence.value);
    VK_CHECK(m_Device->resetFences(1, &f->fence.get()));
    return f;
  }
}
