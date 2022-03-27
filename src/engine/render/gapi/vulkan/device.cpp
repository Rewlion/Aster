#include "device.h"
#include "gapi_to_vk.h"
#include "resources.h"
#include "result.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <cstring>

namespace gapi::vulkan
{
  Device::Device(CreateInfo&& ci)
    : m_Device(std::move(ci.device))
    , m_QueueIndices(ci.queueIndices)
    , m_MemoryIndices(ci.memoryIndices)
  {
    vk::Instance& instance = ci.instance;

    Swapchain::CreateInfo swpCi;
    swpCi.surface = ci.surface;
    swpCi.device = m_Device.get();
    swpCi.presentQueue = m_Device->getQueue(ci.queueIndices.present, 0);
    swpCi.surfaceCapabilities = ci.surfaceCapabilities;
    swpCi.surfaceFormats = ci.surfaceFormats;
    swpCi.surfacePresentModes = ci.surfacePresentModes;
    swpCi.swapchainImageExtent = ci.swapchainImageExtent;
    m_Swapchain = Swapchain(swpCi);

    m_GraphicsQueue = m_Device->getQueue(ci.queueIndices.graphics, 0);
    m_TransferQueue = m_Device->getQueue(ci.queueIndices.transfer, 0);

    const auto createPool = [this](const uint32_t index)
                            {
                              const auto cmdPoolCreateInfo = vk::CommandPoolCreateInfo()
                                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                                .setQueueFamilyIndex(index);

                              return m_Device->createCommandPoolUnique(cmdPoolCreateInfo);
                            };

    m_GraphicsCmdPool = createPool(ci.queueIndices.graphics);
    m_TransferCmdPool = createPool(ci.queueIndices.transfer);
  }

  vk::CommandBuffer Device::allocateCmdBuffer(vk::CommandPool pool)
  {
    const auto allocInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

    vk::CommandBuffer cmdBuf = m_Device->allocateCommandBuffers(allocInfo)[0];

    cmdBuf.begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    );

    return cmdBuf;
  }

  vk::CommandBuffer Device::allocateGraphicsCmdBuffer()
  {
    return allocateCmdBuffer(*m_GraphicsCmdPool);
  }

  vk::CommandBuffer Device::allocateTransferCmdBuffer()
  {
    return allocateCmdBuffer(*m_TransferCmdPool);
  }

  vk::Format Device::getTextureFormat(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.getSurfaceFormat();

    ASSERT(!"UNSUPPORTED");
    return vk::Format::eUndefined;
  }

  vk::ImageView Device::getImageView(const TextureHandler handler)
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

  vk::Image Device::getImage(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type != (uint64_t)TextureType::Allocated)
    {
      ASSERT(!"Can't get not allocated image");
    }

     return m_AllocatedTextures.get(h.as.typed.id).img.get();
  }

  vk::Extent3D Device::getImageDim(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
    {
      auto dim = m_Swapchain.getSurfaceExtent();
      return {dim.width, dim.height, 0};
    }

    ASSERT(!"UNSUPPORTED");
    return {0,0,0};
  }

  void Device::submitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf)
  {
    vk::SubmitInfo submit;
    submit.pCommandBuffers = &cmdBuf;
    submit.commandBufferCount = 1;
    m_GraphicsQueue.submit(submit);
  }

  void Device::transitSurfaceImageForPresent()
  {
    auto cmdBuf = allocateGraphicsCmdBuffer();

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    const auto layoutBarrier = vk::ImageMemoryBarrier()
      .setOldLayout(vk::ImageLayout::eUndefined)
      .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
      .setImage(m_Swapchain.getSurfaceImage())
      .setSubresourceRange(subresourceRange);

    cmdBuf.pipelineBarrier(
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    cmdBuf.end();

    const auto* renderFinishedSemaphore = m_Swapchain.getWaitForRenderFinishedSemaphore();
    const auto submitInfo = vk::SubmitInfo()
      .setPCommandBuffers(&cmdBuf)
      .setCommandBufferCount(1)
      .setPSignalSemaphores(renderFinishedSemaphore)
      .setSignalSemaphoreCount(1);

    m_GraphicsQueue.submit(submitInfo);
  }

  void Device::presentSurfaceImage()
  {
    transitSurfaceImageForPresent();
    m_Swapchain.Present();
  }

  BufferHandler Device::allocateBuffer(const size_t size, const int usage)
  {
    Buffer buffer = allocateBufferInternal(size, usage);

    size_t id = (size_t)(~0);
    const bool allocated = m_AllocatedBuffers.add(std::move(buffer), id);

    ASSERT(allocated);
    return (BufferHandler)id;
  }

  void* Device::mapBuffer(const BufferHandler buffer, const size_t offset, const size_t size)
  {
    Buffer* b = getAllocatedBuffer(buffer);
    if (!(b->usage & BF_CpuVisible))
    {
      ASSERT(!"can't map not cpu visible buffer");
      return nullptr;
    }

    return mapBuffer(*b, offset, size);
  }

  void* Device::mapBuffer(const Buffer& buffer, const size_t offset, const size_t size)
  {
    void* mem = nullptr;
    VK_CHECK(m_Device->mapMemory(buffer.memory.get(), offset, size, vk::MemoryMapFlagBits{}, &mem));

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

    vk::BufferCreateInfo bufferCi;
    bufferCi.usage = get_buffer_usage(usage);
    bufferCi.size = size;
    bufferCi.sharingMode = vk::SharingMode::eExclusive;

    b.buffer = m_Device->createBufferUnique(bufferCi);
    const vk::MemoryRequirements memRec = m_Device->getBufferMemoryRequirements(b.buffer.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRec.size;
    allocInfo.memoryTypeIndex = get_memory_index(m_MemoryIndices, usage);

    b.memory = m_Device->allocateMemoryUnique(allocInfo);

    m_Device->bindBufferMemory(b.buffer.get(), b.memory.get(), 0);

    return b;
  }

  Buffer Device::allocateStagingBuffer(const void* src, const size_t size)
  {
    Buffer stagingBuf = allocateBufferInternal(size, BF_CpuVisible);
    void* mappedMemory = nullptr;

    ASSERT(vk::Result::eSuccess == m_Device->mapMemory(stagingBuf.memory.get(), 0, size, vk::MemoryMapFlagBits{}, &mappedMemory));
    std::memcpy(mappedMemory, src, size);
    m_Device->unmapMemory(stagingBuf.memory.get());

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

  void Device::copyBuffersSync(const BufferHandler src, const size_t srcOffset,
                               const BufferHandler dst, const size_t dstOffset,
                               const size_t size)
  {
    const vk::Buffer srcBuf = getBuffer(src);
    const vk::Buffer dstBuf = getBuffer(dst);

    copyBuffersSync(srcBuf, srcOffset, dstBuf, dstOffset, size);
  }

  void Device::copyBuffersSync(const vk::Buffer src, const size_t srcOffset,
                               const vk::Buffer dst, const size_t dstOffset,
                               const size_t size)
  {
    const vk::CommandBuffer cmdBuf = allocateTransferCmdBuffer();
    vk::BufferCopy region;
    region.size = size;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    cmdBuf.copyBuffer(src, dst, 1, &region);

    cmdBuf.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    vk::UniqueFence fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    VK_CHECK(m_Device->resetFences(1, &fence.get()));
    VK_CHECK(m_TransferQueue.submit(1, &submitInfo, fence.get()));
    VK_CHECK(m_Device->waitForFences(1, &fence.get(), true, ~0));
  }

  void Device::writeToStagingBuffer(const Buffer& buffer, const void* src, const size_t offset, const size_t size)
  {
    void* dst = mapBuffer(buffer, offset, size);
    std::memcpy(dst, src, size);
    unmapBuffer(buffer);
  }

  void Device::writeBuffer(const BufferHandler buffer, const void* src, const size_t offset, const size_t size)
  {
    Buffer* b = getAllocatedBuffer(buffer);

    if (b->usage & BF_CpuVisible)
    {
      writeToStagingBuffer(*b, src, offset, size);
    }
    else if (b->usage & BF_GpuVisible)
    {
      Buffer staging = allocateBufferInternal(size, BF_CpuVisible);
      writeToStagingBuffer(staging, src, 0, size);
      copyBuffersSync(staging.buffer.get(), 0, b->buffer.get(), offset, size);
    }
    else
      ASSERT(!"writeBuffer dst buffer don't have memory set");
  }

  vk::Buffer Device::getBuffer(const BufferHandler buffer)
  {
    const size_t id = (size_t)buffer;
    return m_AllocatedBuffers.get(id).buffer.get();
  }

  TextureHandler Device::allocateTexture(const TextureAllocationDescription& allocDesc)
  {
    Texture resource;
    resource.size = allocDesc.extent;

    vk::ImageCreateInfo ci{};

    ci.imageType = allocDesc.extent.y <= 1 && allocDesc.extent.z <= 1
                  ? vk::ImageType::e1D
                  : allocDesc.extent.z <= 1
                    ? vk::ImageType::e2D
                    : vk::ImageType::e3D;

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

    resource.img = m_Device->createImageUnique(ci);
    resource.currentLayout = ci.initialLayout;

    const vk::MemoryRequirements memRec = m_Device->getImageMemoryRequirements(resource.img.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRec.size;
    allocInfo.memoryTypeIndex = m_MemoryIndices.deviceLocalMemory;

    resource.memory = m_Device->allocateMemoryUnique(allocInfo);

    m_Device->bindImageMemory(resource.img.get(), resource.memory.get(), 0);

    vk::ImageSubresourceRange subresRange;
    subresRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    subresRange.baseArrayLayer = 0;
    subresRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresRange.baseMipLevel = 0;
    subresRange.levelCount = VK_REMAINING_MIP_LEVELS;
    vk::ImageViewCreateInfo viewCi;
    viewCi.image = resource.img.get();
    viewCi.viewType = vk::ImageViewType::e2D;
    viewCi.format = ci.format;
    viewCi.components = vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
    viewCi.subresourceRange = subresRange;

    resource.view = m_Device->createImageViewUnique(viewCi);

    size_t id = ~0;
    m_AllocatedTextures.add(std::move(resource), id);

    TextureHandlerInternal handler;
    handler.as.typed.id = id;
    handler.as.typed.type = (uint64_t)TextureType::Allocated;

    return handler;
  }

  void Device::copyToTextureSync(const void* src, const size_t size, const TextureHandler texture)
  {
    TextureHandlerInternal handler;
    handler.as.handler = uint64_t(texture);
    uint32_t textureId = handler.as.typed.id;

    if (!m_AllocatedTextures.contains(textureId))
    {
      logerror("vulkan: copy_to_texture_sync: texture({}) not allocated", textureId);
      return;
    }

    Buffer stagingBuf = allocateStagingBuffer(src, size);
    const Texture& toTexture = m_AllocatedTextures.get(textureId);

    vk::ImageLayout currentLayout = toTexture.currentLayout;

    const vk::CommandBuffer cmdBuf = allocateTransferCmdBuffer();

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    auto layoutBarrier = vk::ImageMemoryBarrier()
      .setOldLayout(currentLayout)
      .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
      .setImage(toTexture.img.get())
      .setSubresourceRange(subresourceRange);

    cmdBuf.pipelineBarrier(
      vk::PipelineStageFlagBits::eTransfer,
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    vk::BufferImageCopy copyDesc;
    copyDesc.bufferOffset = 0;
    copyDesc.bufferRowLength = 0;
    copyDesc.bufferImageHeight = 0;
    copyDesc.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    copyDesc.imageSubresource.mipLevel = 0;
    copyDesc.imageSubresource.baseArrayLayer = 0;
    copyDesc.imageSubresource.layerCount = 1;
    copyDesc.imageOffset = vk::Offset3D{0,0,0};
    copyDesc.imageExtent = vk::Extent3D{(uint32_t)toTexture.size.x, (uint32_t)toTexture.size.y, 1};

    cmdBuf.copyBufferToImage(stagingBuf.buffer.get(), toTexture.img.get(), vk::ImageLayout::eTransferDstOptimal, 1, &copyDesc);

    layoutBarrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
    layoutBarrier.newLayout = currentLayout;

    cmdBuf.pipelineBarrier(
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::PipelineStageFlagBits::eFragmentShader,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    cmdBuf.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    vk::UniqueFence fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    ASSERT(vk::Result::eSuccess == m_Device->resetFences(1, &fence.get()));

    ASSERT(vk::Result::eSuccess == m_TransferQueue.submit(1, &submitInfo, fence.get()));

    ASSERT(vk::Result::eSuccess == m_Device->waitForFences(1, &fence.get(), true, ~0));
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
    sampler.sampler = m_Device->createSamplerUnique(samplerCi);

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

  vk::ImageLayout Device::getImageLayout(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type != (uint64_t)TextureType::Allocated)
    {
      ASSERT(!"Can't get image layout for not allocated texture");
    }

    return m_AllocatedTextures.get(h.as.typed.id).currentLayout;
  }

  void Device::setImageLayout(const TextureHandler handler, const vk::ImageLayout layout)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type != (uint64_t)TextureType::Allocated)
    {
      ASSERT(!"Can't set image layout for not allocated texture");
    }

    m_AllocatedTextures.get(h.as.typed.id).currentLayout = layout;
  }
}
