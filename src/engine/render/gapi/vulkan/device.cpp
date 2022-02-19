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
}
