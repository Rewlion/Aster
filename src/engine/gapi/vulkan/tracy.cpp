#include "tracy.h"

#include "vulkan.h"
#include "result.h"

namespace gapi::vulkan
{
  tracy::VkCtx* tracyCtx;
  vk::UniqueCommandBuffer tracyCmdBuf;

  void tracy_init(const vk::PhysicalDevice physical_device, const vk::Device device,
                  const vk::CommandPool cmd_pool, const vk::Queue queue)
  {
    const auto allocInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(cmd_pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

    auto r = device.allocateCommandBuffersUnique(allocInfo);
    VK_CHECK_RES(r);
    tracyCmdBuf = std::move(r.value[0]);

    tracyCtx = TracyVkContext(physical_device, device, queue, tracyCmdBuf.get());
  }

  void tracy_destroy()
  {
    tracyCmdBuf = vk::UniqueCommandBuffer{};
    TracyVkDestroy(tracyCtx);
  }

  void tracy_collect_time_stamps(vk::CommandBuffer cmd_buf)
  {
    TracyVkCollect(tracyCtx, cmd_buf)
  }

  auto tracy_get_ctx() -> void*
  {
    return tracyCtx;
  }
}