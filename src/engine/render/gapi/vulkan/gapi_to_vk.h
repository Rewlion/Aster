#pragma once

#include <engine/render/gapi/resources.h>

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  inline vk::CompareOp get_compare_op(const CompareOp op)
  {
    return static_cast<vk::CompareOp>(op);
  }

  inline vk::StencilOp get_stencil_op(const StencilOp op)
  {
    return static_cast<vk::StencilOp>(op);
  }

  inline vk::PrimitiveTopology get_primitive_topology(const PrimitiveTopology topology)
  {
    return static_cast<vk::PrimitiveTopology>(topology);
  }

  inline vk::PipelineDepthStencilStateCreateInfo get_depth_stencil_state(const DepthStencilStateDescription& state)
  {
    vk::StencilOpState stencilOpState;
    stencilOpState.failOp = get_stencil_op(state.stencilFailOp);
    stencilOpState.passOp = get_stencil_op(state.stencilPassOp);
    stencilOpState.depthFailOp = get_stencil_op(state.stencilDepthFailOp);
    stencilOpState.compareOp = get_compare_op(state.stencilCompareOp);
    stencilOpState.compareMask = (uint32_t)-1;
    stencilOpState.writeMask = (uint32_t)-1;
    stencilOpState.reference = state.stencilReferenceValue;

    vk::PipelineDepthStencilStateCreateInfo s;
    s.depthTestEnable = state.depthTestEnabled;
    s.depthWriteEnable = state.depthWriteEnabled;
    s.depthCompareOp = get_compare_op(state.depthOp);
    s.depthBoundsTestEnable = false;
    s.stencilTestEnable = state.stencilTestEnabled;
    s.front = stencilOpState;
    s.back = stencilOpState;
    s.minDepthBounds = 0.0f;
    s.maxDepthBounds = 0.0f;

    return s;
  }

  inline vk::LogicOp get_logic_op(const LogicOp op)
  {
    return static_cast<vk::LogicOp>(op);
  }

  inline vk::BlendFactor get_blend_factor(const BlendFactor f)
  {
    return static_cast<vk::BlendFactor>(f);
  }

  inline vk::BlendOp get_blend_op(const BlendOp op)
  {
    return static_cast<vk::BlendOp>(op);
  }

  inline vk::PipelineColorBlendAttachmentState get_attachment_blend_state(const AttachmentBlendState& state)
  {
    vk::PipelineColorBlendAttachmentState ret;
    ret.blendEnable = state.blendEnabled;
    ret.srcColorBlendFactor = get_blend_factor(state.srcColorBlendFactor);
    ret.dstColorBlendFactor = get_blend_factor(state.dstColorBlendFactor);
    ret.colorBlendOp = get_blend_op(state.colorBlendOp);
    ret.srcAlphaBlendFactor = get_blend_factor(state.srcAlphaBlendFactor);
    ret.dstAlphaBlendFactor = get_blend_factor(state.dstAlphaBlendFactor);
    ret.alphaBlendOp = get_blend_op(state.alphaBlendOp);
    ret.colorWriteMask = vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR
                       | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB;

    return ret;
  }

  inline vk::PipelineColorBlendStateCreateInfo get_blend_state(const BlendState& state, vk::PipelineColorBlendAttachmentState attachmentStates[MAX_RENDER_TARGETS])
  {
    vk::PipelineColorBlendStateCreateInfo ret;
    ret.logicOpEnable = state.logicOpEnabled;
    ret.logicOp = get_logic_op(state.logicOp);
    ret.blendConstants[0] = state.blendConstants[0];
    ret.blendConstants[1] = state.blendConstants[1];
    ret.blendConstants[2] = state.blendConstants[2];
    ret.blendConstants[3] = state.blendConstants[3];

    for (size_t i = 0; i < state.attachments.getSize(); ++i)
      attachmentStates[i] = get_attachment_blend_state(state.attachments.get(i));
    ret.pAttachments = attachmentStates;
    ret.attachmentCount = state.attachments.getSize();

    return ret;
  }

  inline vk::BufferUsageFlags get_buffer_usage(const int usage)
  {
    vk::BufferUsageFlags bits = static_cast<vk::BufferUsageFlagBits>(usage);

    bits |=  vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;

    if (usage & BufferUsage::BF_BindIndex)
      bits |= vk::BufferUsageFlagBits::eIndexBuffer;

    if (usage & BufferUsage::BF_BindVertex)
      bits |= vk::BufferUsageFlagBits::eVertexBuffer;

    if (usage & BufferUsage::BF_BindConstant)
      bits |= vk::BufferUsageFlagBits::eUniformBuffer;

    return bits;
  }

  inline uint32_t get_buffer_discards_count(const int usage)
  {
    if (usage & BF_BindConstant)
      return 3;

    return 1;
  }

  inline uint32_t get_memory_index(const MemoryIndices memoryIndices, const int bufferUsage)
  {
    if (bufferUsage & BF_GpuVisible)
      return memoryIndices.deviceLocalMemory;

    if (bufferUsage & BF_CpuVisible)
      return memoryIndices.stagingMemory;

    ASSERT(!"buffer usage has to specify memory storage");

    return 0;
  }

  inline vk::ShaderStageFlagBits get_shader_stage(const ShaderStage stage)
  {
    return static_cast<vk::ShaderStageFlagBits>(stage);
  }

  inline vk::Format get_image_format(const TextureFormat format)
  {
    switch (format)
    {
      case TextureFormat::R8G8B8A8_SNORM: return vk::Format::eR8G8B8A8Snorm;
      case TextureFormat::R8G8B8A8_UNORM: return vk::Format::eR8G8B8A8Unorm;
      default: return vk::Format::eUndefined;
    }
  }

  inline vk::ImageUsageFlags get_texture_usage(const TextureUsage usage)
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

  inline vk::SampleCountFlagBits get_image_sample_count(const TextureSamples samples)
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

  inline vk::Filter get_filter(const ImageFilter filter)
  {
    switch (filter)
    {
      case ImageFilter::Nearest: vk::Filter::eNearest;
      case ImageFilter::Linear:  vk::Filter::eLinear;
      case ImageFilter::Cubic:   vk::Filter::eCubicEXT;

      default: return {};
    }
  }

  inline vk::SamplerMipmapMode get_sampler_mimpmap_mode(const SamplerMipmapMode mode)
  {
    return static_cast<vk::SamplerMipmapMode>(mode);
  }

  inline vk::SamplerAddressMode get_sampler_address_mode(const SamplerAddressMode mode)
  {
    return static_cast<vk::SamplerAddressMode>(mode);
  }

  inline vk::BorderColor get_border_color(const BorderColor color)
  {
    return static_cast<vk::BorderColor>(color);
  }
}