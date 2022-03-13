#include "device.h"
#include "resources.h"
#include "gapi_to_vk.h"

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

  vk::CommandBuffer Device::AllocateCmdBuffer(vk::CommandPool pool)
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

  vk::CommandBuffer Device::AllocateGraphicsCmdBuffer()
  {
    return AllocateCmdBuffer(*m_GraphicsCmdPool);
  }

  vk::CommandBuffer Device::AllocateTransferCmdBuffer()
  {
    return AllocateCmdBuffer(*m_TransferCmdPool);
  }

  vk::Format Device::getTextureFormat(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.GetSurfaceFormat();

    ASSERT(!"UNSUPPORTED");
    return vk::Format::eUndefined;
  }

  vk::ImageView Device::getImageView(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_Swapchain.GetSurfaceImageView();

    if (h.as.typed.type == (uint64_t)TextureType::Allocated)
    {
      ASSERT(m_AllocatedTextures.Contains(h.as.typed.id));
      return m_AllocatedTextures.Get(h.as.typed.id).view.get();
    }

    ASSERT(!"UNSUPPORTED");
    return vk::ImageView{};
  }

  vk::Extent3D Device::GetImageDim(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
    {
      auto dim = m_Swapchain.GetSurfaceExtent();
      return {dim.width, dim.height, 0};
    }

    ASSERT(!"UNSUPPORTED");
    return {0,0,0};
  }

  void Device::SubmitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf)
  {
    vk::SubmitInfo submit;
    submit.pCommandBuffers = &cmdBuf;
    submit.commandBufferCount = 1;
    m_GraphicsQueue.submit(submit);
  }

  void Device::TransitSurfaceImageForPresent()
  {
    auto cmdBuf = AllocateGraphicsCmdBuffer();

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    const auto layoutBarrier = vk::ImageMemoryBarrier()
      .setOldLayout(vk::ImageLayout::eUndefined)
      .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
      .setImage(m_Swapchain.GetSurfaceImage())
      .setSubresourceRange(subresourceRange);

    cmdBuf.pipelineBarrier(
      vk::PipelineStageFlagBits::eAllCommands,
      vk::PipelineStageFlagBits::eAllCommands,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    cmdBuf.end();

    const auto* renderFinishedSemaphore = m_Swapchain.GetWaitForRenderFinishedSemaphore();
    const auto submitInfo = vk::SubmitInfo()
      .setPCommandBuffers(&cmdBuf)
      .setCommandBufferCount(1)
      .setPSignalSemaphores(renderFinishedSemaphore)
      .setSignalSemaphoreCount(1);

    m_GraphicsQueue.submit(submitInfo);
  }

  void Device::PresentSurfaceImage()
  {
    TransitSurfaceImageForPresent();
    m_Swapchain.Present();
  }

  BufferHandler Device::AllocateBuffer(const BufferAllocationDescription& allocDesc)
  {
    Buffer buffer = AllocateBufferInternal(allocDesc, m_MemoryIndices.deviceLocalMemory);

    size_t id = (size_t)(~0);
    const bool allocated = m_AllocatedBuffers.Add(std::move(buffer), id);

    ASSERT(allocated);
    return (BufferHandler)id;
  }

  Buffer Device::AllocateBufferInternal(const BufferAllocationDescription& allocDesc, const uint32_t memoryIndex)
  {
    Buffer b;

    vk::BufferCreateInfo bufferCi;
    bufferCi.usage = GetBufferUsage(allocDesc.usage);
    bufferCi.size = allocDesc.size;
    bufferCi.sharingMode = vk::SharingMode::eExclusive;

    b.buffer = m_Device->createBufferUnique(bufferCi);
    const vk::MemoryRequirements memRec = m_Device->getBufferMemoryRequirements(b.buffer.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRec.size;
    allocInfo.memoryTypeIndex = memoryIndex;

    b.memory = m_Device->allocateMemoryUnique(allocInfo);

    m_Device->bindBufferMemory(b.buffer.get(), b.memory.get(), 0);

    return b;
  }

  Buffer Device::AllocateStagingBuffer(const size_t size)
  {
    BufferAllocationDescription allocDesc;
    allocDesc.size = size;
    allocDesc.usage = gapi::BufferUsage::Staging;
    return AllocateBufferInternal(allocDesc, m_MemoryIndices.stagingMemory);
  }

  Buffer Device::AllocateStagingBuffer(const void* src, const size_t size)
  {
    Buffer stagingBuf = AllocateStagingBuffer(size);
    void* mappedMemory = nullptr;

    ASSERT(vk::Result::eSuccess == m_Device->mapMemory(stagingBuf.memory.get(), 0, size, vk::MemoryMapFlagBits{}, &mappedMemory));
    std::memcpy(mappedMemory, src, size);
    m_Device->unmapMemory(stagingBuf.memory.get());

    return stagingBuf;
  }

  void Device::CopyToBufferSync(const void* src, const size_t offset, const size_t size, const BufferHandler buffer)
  {
    const size_t id = (size_t)buffer;
    if (!m_AllocatedBuffers.Contains(id))
    {
      logerror("vulkan: CopyToBufferSync: buffer({}) not allocated", id);
      return;
    }

    Buffer stagingBuf = AllocateStagingBuffer(size);
    void* mappedMemory = nullptr;

    ASSERT(vk::Result::eSuccess == m_Device->mapMemory(stagingBuf.memory.get(), 0, size, vk::MemoryMapFlagBits{}, &mappedMemory));
    std::memcpy(mappedMemory, src, size);
    m_Device->unmapMemory(stagingBuf.memory.get());

    const Buffer& toBuf = m_AllocatedBuffers.Get(id);

    const vk::CommandBuffer cmdBuf = AllocateTransferCmdBuffer();
    vk::BufferCopy region;
    region.size = size;
    region.srcOffset = 0;
    region.dstOffset = offset;
    cmdBuf.copyBuffer(stagingBuf.buffer.get(), toBuf.buffer.get(), 1, &region);

    cmdBuf.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;

    vk::UniqueFence fence = m_Device->createFenceUnique(vk::FenceCreateInfo{});
    ASSERT(vk::Result::eSuccess == m_Device->resetFences(1, &fence.get()));

    ASSERT(vk::Result::eSuccess == m_TransferQueue.submit(1, &submitInfo, fence.get()));

    ASSERT(vk::Result::eSuccess == m_Device->waitForFences(1, &fence.get(), true, ~0));
  }

  vk::Buffer Device::GetBuffer(const BufferHandler buffer)
  {
    const size_t id = (size_t)buffer;
    return m_AllocatedBuffers.Get(id).buffer.get();
  }

  TextureHandler Device::AllocateTexture(const TextureAllocationDescription& allocDesc)
  {
    Texture resource;
    resource.size = allocDesc.extent;

    vk::ImageCreateInfo ci{};

    ci.imageType = allocDesc.extent.y <= 1 && allocDesc.extent.z <= 1
                  ? vk::ImageType::e1D
                  : allocDesc.extent.z <= 1
                    ? vk::ImageType::e2D
                    : vk::ImageType::e3D;

    ci.format = GetImageFormat(allocDesc.format);
    ci.extent = vk::Extent3D{(uint32_t)allocDesc.extent.x, (uint32_t)allocDesc.extent.y, (uint32_t)allocDesc.extent.z};
    ci.mipLevels = allocDesc.mipLevels;
    ci.arrayLayers = allocDesc.arrayLayers;
    ci.samples = GetImageSampleCount(allocDesc.samplesPerPixel);
    ci.tiling = vk::ImageTiling::eOptimal;
    ci.usage = GetTextureUsage(allocDesc.usage);
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
    m_AllocatedTextures.Add(std::move(resource), id);

    TextureHandlerInternal handler;
    handler.as.typed.id = id;
    handler.as.typed.type = (uint64_t)TextureType::Allocated;

    return handler;
  }

  void Device::CopyToTextureSync(const void* src, const size_t size, const TextureHandler texture)
  {
    TextureHandlerInternal handler;
    handler.as.handler = uint64_t(texture);
    uint32_t textureId = handler.as.typed.id;

    if (!m_AllocatedTextures.Contains(textureId))
    {
      logerror("vulkan: CopyToTextureSync: texture({}) not allocated", textureId);
      return;
    }

    Buffer stagingBuf = AllocateStagingBuffer(src, size);
    const Texture& toTexture = m_AllocatedTextures.Get(textureId);

    vk::ImageLayout currentLayout = toTexture.currentLayout;

    const vk::CommandBuffer cmdBuf = AllocateTransferCmdBuffer();

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
    layoutBarrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal; //TMP HACK

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

  void Device::ImageBarrier(vk::CommandBuffer& cmdBuf, const TextureHandler handler, const vk::ImageLayout newLayout,
                            const vk::PipelineStageFlagBits srcStage, const vk::PipelineStageFlagBits dstStage)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
    {
      logerror("vulkan: unable to set image barrier for swapchain image");
      return;
    }

    if (h.as.typed.type != (uint64_t)TextureType::Allocated)
    {
      ASSERT(!"Unsupported");
    }

    Texture& texture = m_AllocatedTextures.Get(h.as.typed.id);

    if (texture.currentLayout == newLayout)
      return;

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;

    vk::ImageMemoryBarrier layoutBarrier;
    layoutBarrier.oldLayout = texture.currentLayout;
    layoutBarrier.newLayout;
    layoutBarrier.image = texture.img.get();
    layoutBarrier.subresourceRange = subresourceRange;

    cmdBuf.pipelineBarrier(
      srcStage,
      dstStage,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    texture.currentLayout = newLayout;
  }

  SamplerHandler Device::AllocateSampler(const SamplerAllocationDescription& allocDesc)
  {
    vk::SamplerCreateInfo samplerCi;
    samplerCi.magFilter = GetFilter(allocDesc.magFilter);
    samplerCi.minFilter = GetFilter(allocDesc.minFilter);
    samplerCi.mipmapMode = GetSamplerMimpmapMode(allocDesc.mipmapMode);
    samplerCi.addressModeU = GetSamplerAddressMode(allocDesc.addressModeU);
    samplerCi.addressModeV = GetSamplerAddressMode(allocDesc.addressModeV);
    samplerCi.addressModeW = GetSamplerAddressMode(allocDesc.addressModeW);
    samplerCi.mipLodBias = allocDesc.mipLodBias;
    samplerCi.anisotropyEnable = allocDesc.anisotropyEnable;
    samplerCi.maxAnisotropy = allocDesc.maxAnisotropy;
    samplerCi.compareEnable = allocDesc.compareEnable;
    samplerCi.compareOp = getCompareOp(allocDesc.compareOp);
    samplerCi.minLod = allocDesc.minLod;
    samplerCi.maxLod = allocDesc.maxLod;
    samplerCi.borderColor = GetBorderColor(allocDesc.borderColor);
    samplerCi.unnormalizedCoordinates = allocDesc.unnormalizedCoordinates;

    Sampler sampler;
    sampler.sampler = m_Device->createSamplerUnique(samplerCi);

    size_t id = (size_t)(~0);
    const bool allocated = m_AllocatedSamplers.Add(std::move(sampler), id);

    ASSERT(allocated);

    return SamplerHandler(id);
  }

  vk::Sampler Device::GetSampler(const SamplerHandler sampler)
  {
    const size_t id = static_cast<size_t>(sampler);
    if (!m_AllocatedSamplers.Contains(id))
    {
      ASSERT(!"Failed to get sampler: not allocated");
      return {};
    }

    return m_AllocatedSamplers.Get(id).sampler.get();
  }
}
