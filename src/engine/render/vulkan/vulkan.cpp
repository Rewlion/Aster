#pragma once

#include "vulkan.h"
#include "backend.h"

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  static Backend backend;
  static Device device;

  void init()
  {
    backend.init();
    device = backend.createDevice();
  }
}