#pragma once

#ifdef PLATFORM_WIN64
  #define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#ifndef VULKAN_HPP_ASSERT_ON_RESULT
  #define VULKAN_HPP_ASSERT_ON_RESULT(b)
#endif
#define VULKAN_HPP_NO_EXCEPTIONS 1

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdefaulted-function-deleted"
#include <vulkan/vulkan.hpp>
#pragma clang diagnostic pop

namespace vk
{
  class Device;
}

namespace gapi::vulkan
{
  void init();

  vk::Device& get_device();
}
