#include "device.h"
#include "resources.h"

#include <engine/assert.h>

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

    const auto cmdPoolCreateInfo = vk::CommandPoolCreateInfo()
      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
      .setQueueFamilyIndex(ci.queueIndices.graphics);

    m_GraphicsCmdPool = m_Device->createCommandPoolUnique(cmdPoolCreateInfo);
  }

  vk::CommandBuffer Device::allocateGraphicsCmdBuffer()
  {
    const auto allocInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(*m_GraphicsCmdPool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

    vk::CommandBuffer cmdBuf = m_Device->allocateCommandBuffers(allocInfo)[0];

    cmdBuf.begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    );

    return cmdBuf;
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
}
