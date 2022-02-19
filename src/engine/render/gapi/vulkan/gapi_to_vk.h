#pragma once

#include <engine/render/gapi/resources.h>

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

  inline vk::LogicOp GetLogicOp(const LogicOp op)
  {
    return static_cast<vk::LogicOp>(op);
  }

  inline vk::BlendFactor GetBlendFactor(const BlendFactor f)
  {
    return static_cast<vk::BlendFactor>(f);
  }

  inline vk::BlendOp GetBlendOp(const BlendOp op)
  {
    return static_cast<vk::BlendOp>(op);
  }

  inline vk::PipelineColorBlendAttachmentState GetAttachmentBlendState(const AttachmentBlendState& state)
  {
    vk::PipelineColorBlendAttachmentState ret;
    ret.blendEnable = state.blendEnabled;
    ret.srcColorBlendFactor = GetBlendFactor(state.srcColorBlendFactor);
    ret.dstColorBlendFactor = GetBlendFactor(state.dstColorBlendFactor);
    ret.colorBlendOp = GetBlendOp(state.colorBlendOp);
    ret.srcAlphaBlendFactor = GetBlendFactor(state.srcAlphaBlendFactor);
    ret.dstAlphaBlendFactor = GetBlendFactor(state.dstAlphaBlendFactor);
    ret.alphaBlendOp = GetBlendOp(state.alphaBlendOp);
    ret.colorWriteMask = vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR
                       | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB;

    return ret;
  }

  inline vk::PipelineColorBlendStateCreateInfo GetBlendState(const BlendState& state, vk::PipelineColorBlendAttachmentState attachmentStates[MAX_RENDER_TARGETS])
  {
    vk::PipelineColorBlendStateCreateInfo ret;
    ret.logicOpEnable = state.logicOpEnabled;
    ret.logicOp = GetLogicOp(state.logicOp);
    ret.blendConstants[0] = state.blendConstants[0];
    ret.blendConstants[1] = state.blendConstants[1];
    ret.blendConstants[2] = state.blendConstants[2];
    ret.blendConstants[3] = state.blendConstants[3];

    ret.attachmentCount = state.attachmentsCount;
    for (size_t i = 0; i < state.attachmentsCount; ++i)
      attachmentStates[i] = GetAttachmentBlendState(state.attachments[i]);
    ret.pAttachments = attachmentStates;

    return ret;
  }

  inline vk::BufferUsageFlags GetBufferUsage(const BufferUsage usage)
  {
    vk::BufferUsageFlags bits = static_cast<vk::BufferUsageFlagBits>(usage);
    switch(usage)
    {
      case BufferUsage::Staging:
      {
        bits = vk::BufferUsageFlagBits::eTransferSrc;
        break;
      }
      case BufferUsage::Vertex:
      case BufferUsage::Index:
      {
        bits |= vk::BufferUsageFlagBits::eTransferDst;
        break;
      }
    }
    return bits;
  }

  inline vk::ShaderStageFlagBits GetShaderStage(const ShaderStage stage)
  {
    return static_cast<vk::ShaderStageFlagBits>(stage);
  }
}