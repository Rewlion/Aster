#pragma once

#ifndef VULKAN_HPP_ASSERT_ON_RESULT
  #define VULKAN_HPP_ASSERT_ON_RESULT(b)
#endif
#define VULKAN_HPP_NO_EXCEPTIONS 1
#include <vulkan/vulkan.hpp>

namespace vk
{
  class Device;
}

namespace gapi::vulkan
{
  void init();

  vk::Device& get_device();
}
