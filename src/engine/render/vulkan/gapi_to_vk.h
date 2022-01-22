#pragma once

#include <engine/render/resources.h>

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  inline vk::CompareOp getCompareOp(const CompareOp op)
  {
    return static_cast<vk::CompareOp>(op);
  }

  inline vk::StencilOp getStencilOp(const StencilOp op)
  {
    return static_cast<vk::StencilOp>(op);
  }
}