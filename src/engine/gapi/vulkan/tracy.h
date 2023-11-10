#pragma once

#include "vulkan.h"

#include <tracy/TracyVulkan.hpp>

#ifdef TRACY_ENABLE
namespace gapi::vulkan
{
  void tracy_init(const vk::PhysicalDevice, const vk::Device,
                  const vk::CommandPool, const vk::Queue);

  void tracy_destroy();

  void tracy_collect_time_stamps(vk::CommandBuffer cmd_buf);

  auto tracy_get_ctx() -> tracy::VkCtx*;
}
 
#endif