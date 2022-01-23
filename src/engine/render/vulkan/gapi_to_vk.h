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

  inline vk::PrimitiveTopology GetPrimitiveTopology(const PrimitiveTopology topology)
  {
    return static_cast<vk::PrimitiveTopology>(topology);
  }

  inline vk::PipelineDepthStencilStateCreateInfo GetDepthStencilState(const DepthStencilStateDescription& state)
  {
    vk::StencilOpState stencilOpState;
    stencilOpState.failOp = getStencilOp(state.stencilFailOp);
    stencilOpState.passOp = getStencilOp(state.stencilPassOp);
    stencilOpState.depthFailOp = getStencilOp(state.stencilDepthFailOp);
    stencilOpState.compareOp = getCompareOp(state.stencilCompareOp);
    stencilOpState.compareMask = (uint32_t)-1;
    stencilOpState.writeMask = (uint32_t)-1;
    stencilOpState.reference = state.stencilReferenceValue;

    vk::PipelineDepthStencilStateCreateInfo s;
    s.depthTestEnable = state.depthTestEnabled;
    s.depthWriteEnable = state.depthWriteEnabled;
    s.depthCompareOp = getCompareOp(state.depthOp);
    s.depthBoundsTestEnable = false;
    s.stencilTestEnable = state.stencilTestEnabled;
    s.front = stencilOpState;
    s.back = stencilOpState;
    s.minDepthBounds = 0.0f;
    s.maxDepthBounds = 0.0f;

    return s;
  }
}