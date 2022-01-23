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

    m_SurfaceFormat = getSuitableSurfaceFormat(ci.surfaceFormats);
    m_SurfaceExtent = getSwapchainExtent(ci.surfaceCapabilities, ci.swapchainImageExtent);
    m_Swapchain = createSwapchain(ci, m_SurfaceFormat, m_SurfaceExtent);
    setSwapchainResources();

    m_GraphicsQueue = m_Device->getQueue(ci.queueIndices.graphics, 0);
    m_PresentQueue = m_Device->getQueue(ci.queueIndices.present, 0);
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
    return cmdBuf;
  }

  vk::Format Device::getTextureFormat(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return m_SurfaceFormat.format;

    ASSERT(!"UNSUPPORTED");
    return vk::Format::eUndefined;
  }

  vk::ImageView Device::getImageView(const TextureHandler handler)
  {
    TextureHandlerInternal h{handler};
    if (h.as.typed.type == (uint64_t)TextureType::SurfaceRT)
      return *m_SwapchainResources.views[frameId];

    ASSERT(!"UNSUPPORTED");
    return vk::ImageView{};
  }

  vk::UniqueSwapchainKHR Device::createSwapchain(
    const CreateInfo& ci,
    const vk::SurfaceFormatKHR& surfaceFormat,
    const vk::Extent2D& surfaceExtent) const
  {
    const auto swapchainPresentMode = getSwapchainPresentMode(ci.surfacePresentModes, vk::PresentModeKHR::eFifo);
    const auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
          .setSurface(ci.surface)
          .setMinImageCount(SWAPCHAIN_IMAGES_COUNT)
          .setImageFormat(surfaceFormat.format)
          .setImageColorSpace(surfaceFormat.colorSpace)
          .setImageExtent(surfaceExtent)
          .setImageArrayLayers(1)
          .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
          .setPreTransform(ci.surfaceCapabilities.currentTransform)
          .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
          .setPresentMode(swapchainPresentMode)
          .setClipped(true)
          .setOldSwapchain(nullptr);

    return m_Device->createSwapchainKHRUnique(swapchainCreateInfo);
  }

  vk::SurfaceFormatKHR Device::getSuitableSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) const
  {
    return { vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear };
  }

  vk::PresentModeKHR Device::getSwapchainPresentMode(
        const std::vector<vk::PresentModeKHR>& availablePresentModes,
        const vk::PresentModeKHR& preferredMode) const
  {
    bool hasFifo = false;

    for (const auto& mode : availablePresentModes)
    {
      if (mode == preferredMode)
        return preferredMode;

      if (mode == vk::PresentModeKHR::eFifo)
        hasFifo = true;
    }

    ASSERT(hasFifo);

    return vk::PresentModeKHR::eFifo;
  }

  vk::Extent2D Device::getSwapchainExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& windowSize) const
  {
    if (capabilities.currentExtent.width != uint32_t(-1))
    {
      return capabilities.currentExtent;
    }
    else
    {
      vk::Extent2D actualExtent = windowSize;
      actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
      return actualExtent;
    }
  }

  void Device::setSwapchainResources()
  {
    std::vector<vk::Image> images = m_Device->getSwapchainImagesKHR(*m_Swapchain);
    for (size_t i = 0; i < images.size(); ++i)
    {
      m_SwapchainResources.images[i] = images[i];

      const auto& compMap = vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
      const auto imgViewCi = vk::ImageViewCreateInfo()
        .setImage(images[i])
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(m_SurfaceFormat.format)
        .setComponents(compMap);
      m_SwapchainResources.views[i] = m_Device->createImageViewUnique(imgViewCi);

      m_SwapchainResources.imageAcquiredFence = m_Device->createFenceUnique(vk::FenceCreateInfo{});

      const auto semaphoreTypeCi = vk::SemaphoreTypeCreateInfo()
        .setInitialValue(0)
        .setSemaphoreType(vk::SemaphoreType::eBinary);

      const auto semaphoreCi = vk::SemaphoreCreateInfo()
        .setPNext(&semaphoreTypeCi);

      m_SwapchainResources.renderingFinishedSemaphores[i] = m_Device->createSemaphoreUnique(semaphoreCi);
    }
  }

  void Device::beginFrame()
  {
    frameId = m_Device->acquireNextImageKHR(*m_Swapchain, -1, {}, *m_SwapchainResources.imageAcquiredFence).value;
    const vk::Result r = m_Device->waitForFences(1, &m_SwapchainResources.imageAcquiredFence.get(), true, -1);
    ASSERT(r == vk::Result::eSuccess);
  }

  void Device::SubmitGraphicsCmdBuf(const vk::CommandBuffer& cmdBuf)
  {
    vk::SubmitInfo submit;
    submit.pCommandBuffers = &cmdBuf;
    submit.commandBufferCount = 1;
    m_GraphicsQueue.submit(submit);
  }

  void Device::endFrame()
  {
    auto cmdBuf = allocateGraphicsCmdBuffer();
    cmdBuf.begin(
      vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
    );

    const auto layoutBarrier = vk::ImageMemoryBarrier()
      .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
      .setImage(m_SwapchainResources.images[frameId]);

    cmdBuf.pipelineBarrier(
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::DependencyFlagBits{},
      0, nullptr,
      0, nullptr,
      1, &layoutBarrier);

    cmdBuf.end();

    const auto* renderFinishedSemaphore = &m_SwapchainResources.renderingFinishedSemaphores[frameId].get();
    const auto submitInfo = vk::SubmitInfo()
      .setPCommandBuffers(&cmdBuf)
      .setCommandBufferCount(1)
      .setPSignalSemaphores(renderFinishedSemaphore)
      .setSignalSemaphoreCount(1);

    m_GraphicsQueue.submit(submitInfo);

    vk::SwapchainKHR swapchains[] {m_Swapchain.get()};
    const auto presentInfo = vk::PresentInfoKHR()
      .setSwapchainCount(1)
      .setPSwapchains(swapchains)
      .setPImageIndices(&frameId)
      .setPResults(nullptr)
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(renderFinishedSemaphore);

    const vk::Result r = m_PresentQueue.presentKHR(presentInfo);
    ASSERT(r == vk::Result::eSuccess);
  }
}
