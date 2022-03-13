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

  inline vk::Format GetImageFormat(const TextureFormat format)
  {
    switch (format)
    {
      case TextureFormat::R8G8B8A8_SNORM: return vk::Format::eR8G8B8A8Snorm;
      case TextureFormat::R8G8B8A8_UNORM: return vk::Format::eR8G8B8A8Unorm;
      default: return vk::Format::eUndefined;
    }
  }

  inline vk::ImageUsageFlags GetTextureUsage(const TextureUsage usage)
  {
    vk::ImageUsageFlags bits;

    bits |= vk::ImageUsageFlagBits::eTransferDst;

    uint32_t usg = (uint32_t)(usage);

    if (usg & (uint32_t)TextureUsage::DepthStencil)
      bits |= vk::ImageUsageFlagBits::eDepthStencilAttachment;

    if (usg & (uint32_t)TextureUsage::RenderTarget)
      bits |= vk::ImageUsageFlagBits::eColorAttachment;

    if (usg & (uint32_t)TextureUsage::Uniform)
    {
      bits |= vk::ImageUsageFlagBits::eStorage;
      bits |= vk::ImageUsageFlagBits::eSampled;
    }

    return bits;
  }

  inline vk::SampleCountFlagBits GetImageSampleCount(const TextureSamples samples)
  {
    switch(samples)
    {
      case TextureSamples::s1 : return vk::SampleCountFlagBits::e1;
      case TextureSamples::s2 : return vk::SampleCountFlagBits::e2;
      case TextureSamples::s4 : return vk::SampleCountFlagBits::e4;
      case TextureSamples::s8 : return vk::SampleCountFlagBits::e8;
      case TextureSamples::s16: return vk::SampleCountFlagBits::e16;
      case TextureSamples::s32: return vk::SampleCountFlagBits::e32;
      case TextureSamples::s64: return vk::SampleCountFlagBits::e64;

      default: return vk::SampleCountFlagBits{};
    }
  }

  inline vk::Filter GetFilter(const ImageFilter filter)
  {
    switch (filter)
    {
      case ImageFilter::Nearest: vk::Filter::eNearest;
      case ImageFilter::Linear:  vk::Filter::eLinear;
      case ImageFilter::Cubic:   vk::Filter::eCubicEXT;

      default: return {};
    }
  }

  inline vk::SamplerMipmapMode GetSamplerMimpmapMode(const SamplerMipmapMode mode)
  {
    return static_cast<vk::SamplerMipmapMode>(mode);
  }

  inline vk::SamplerAddressMode GetSamplerAddressMode(const SamplerAddressMode mode)
  {
    return static_cast<vk::SamplerAddressMode>(mode);
  }

  inline vk::BorderColor GetBorderColor(const BorderColor color)
  {
    return static_cast<vk::BorderColor>(color);
  }
}