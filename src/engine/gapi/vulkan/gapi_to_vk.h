#pragma once

#include "vulkan.h"
#include "indices.h"

#include <engine/gapi/resources.h>
#include <engine/platform/memory.h>

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

  inline size_t get_buffer_size(const size_t blockSize, const size_t discards, const int usage, const vk::PhysicalDeviceLimits& limits)
  {
    if (usage & BF_BindConstant)
    {
      const size_t alignment = limits.minUniformBufferOffsetAlignment;
      if (blockSize >= alignment)
      {
        size_t alignedBlock = blockSize;
        Sys::align(alignedBlock, alignment);
        return alignedBlock * discards;
      }
      else
        return alignment * discards;
    }

    return blockSize * discards;
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
      case TextureFormat::R32G32B32A32_S: return vk::Format::eR32G32B32A32Sfloat;
      case TextureFormat::D24_UNORM_S8_UINT: return vk::Format::eD24UnormS8Uint;
      default: return vk::Format::eUndefined;
    }
  }

  inline vk::ImageAspectFlags get_image_aspect_flags(const vk::Format format)
  {
    switch (format)
    {
      case vk::Format::eB8G8R8A8Srgb:
      case vk::Format::eR8G8B8A8Snorm:
      case vk::Format::eR8G8B8A8Unorm: return vk::ImageAspectFlagBits::eColor;
      case vk::Format::eR32G32B32A32Sfloat: return vk::ImageAspectFlagBits::eColor;
      case vk::Format::eD24UnormS8Uint: return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
      default: {
        ASSERT(!"unsupported format");
        return {};
      }
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
      case ImageFilter::Nearest: return vk::Filter::eNearest;
      case ImageFilter::Linear:  return vk::Filter::eLinear;
      case ImageFilter::Cubic:   return vk::Filter::eCubicEXT;

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

  inline vk::AttachmentLoadOp load_op_to_vk(const TextureLoadOp op)
  {
    switch (op)
    {
      case TextureLoadOp::Load: return vk::AttachmentLoadOp::eLoad;
      case TextureLoadOp::Clear: return vk::AttachmentLoadOp::eClear;
      case TextureLoadOp::DontCare: return vk::AttachmentLoadOp::eDontCare;
      default: ASSERT(!"unsupported load op"); return vk::AttachmentLoadOp::eDontCare;
    }
  }

  inline vk::AttachmentStoreOp store_op_to_vk(const TextureStoreOp op)
  {
    switch (op)
    {
      case TextureStoreOp::Store: return vk::AttachmentStoreOp::eStore;
      case TextureStoreOp::DontCare: return vk::AttachmentStoreOp::eDontCare;
      default: ASSERT(!"unsupported load op"); return vk::AttachmentStoreOp::eDontCare;
    }
  }

  inline vk::PipelineStageFlags get_pipeline_stage_for_texture_state(const TextureState state)
  {
    switch (state)
    {
      case TextureState::Undefined:          return vk::PipelineStageFlagBits::eTopOfPipe;
      case TextureState::DepthReadStencilWrite:
      case TextureState::DepthReadStencilRead:
      case TextureState::DepthWriteStencilWrite:
      case TextureState::DepthWriteStencilRead:
      case TextureState::DepthRead:
      case TextureState::StencilRead:
      case TextureState::StencilWrite:
      case TextureState::DepthWrite:         return vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
      case TextureState::RenderTarget:       return vk::PipelineStageFlagBits::eColorAttachmentOutput;
      case TextureState::ShaderRead:         return vk::PipelineStageFlagBits::eAllGraphics;
      case TextureState::Present:            return vk::PipelineStageFlagBits::eColorAttachmentOutput;
      case TextureState::TransferDst:        return vk::PipelineStageFlagBits::eTransfer;
      default: ASSERT(!"unsupported stage"); return {};
    }
  }

  inline vk::ImageLayout get_image_layout_for_texture_state(const TextureState state)
  {
    switch (state)
    {
      case TextureState::Undefined:              return vk::ImageLayout::eUndefined;
      case TextureState::DepthReadStencilWrite:  return vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal;
      case TextureState::DepthReadStencilRead:   return vk::ImageLayout::eDepthStencilReadOnlyOptimal;
      case TextureState::DepthWriteStencilWrite: return vk::ImageLayout::eDepthStencilAttachmentOptimal;
      case TextureState::DepthWriteStencilRead:  return vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal;
      case TextureState::StencilRead:            return vk::ImageLayout::eStencilReadOnlyOptimal;
      case TextureState::StencilWrite:           return vk::ImageLayout::eStencilAttachmentOptimal;
      case TextureState::DepthRead:              return vk::ImageLayout::eDepthReadOnlyOptimal;
      case TextureState::DepthWrite:             return vk::ImageLayout::eDepthAttachmentOptimal;
      case TextureState::RenderTarget:           return vk::ImageLayout::eColorAttachmentOptimal;
      case TextureState::ShaderRead:             return vk::ImageLayout::eShaderReadOnlyOptimal;
      case TextureState::Present:                return vk::ImageLayout::ePresentSrcKHR;
      case TextureState::TransferDst:            return vk::ImageLayout::eTransferDstOptimal;
      default: ASSERT(!"unsupported stage");     return {};
    }
  }

  inline vk::AccessFlags get_image_access_flags(const TextureState state)
  {
    switch (state)
    {
      case TextureState::Undefined:              return {};
      case TextureState::DepthReadStencilWrite:  return vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      case TextureState::DepthReadStencilRead:   return vk::AccessFlagBits::eDepthStencilAttachmentRead;
      case TextureState::DepthWriteStencilWrite: return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      case TextureState::DepthWriteStencilRead:  return vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      case TextureState::StencilRead:            return vk::AccessFlagBits::eDepthStencilAttachmentRead;
      case TextureState::StencilWrite:           return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      case TextureState::DepthRead:              return vk::AccessFlagBits::eDepthStencilAttachmentRead;
      case TextureState::DepthWrite:             return vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      case TextureState::RenderTarget:           return vk::AccessFlagBits::eColorAttachmentWrite;
      case TextureState::ShaderRead:             return vk::AccessFlagBits::eShaderRead;
      case TextureState::Present:                return {};
      case TextureState::TransferDst:            return vk::AccessFlagBits::eTransferWrite;
      default: ASSERT(!"unsupported stage");     return {};
    }
  }

  inline vk::ShaderStageFlagBits get_vk_stage(const gapi::ShaderStage stage)
  {
    switch (stage)
    {
       case gapi::ShaderStage::Vertex: return vk::ShaderStageFlagBits::eVertex;
       case gapi::ShaderStage::TessellationControl: return vk::ShaderStageFlagBits::eTessellationControl;
       case gapi::ShaderStage::TessellationEvaluation: return vk::ShaderStageFlagBits::eTessellationEvaluation;
       case gapi::ShaderStage::Geometry: return vk::ShaderStageFlagBits::eGeometry;
       case gapi::ShaderStage::Fragment: return vk::ShaderStageFlagBits::eFragment;
       case gapi::ShaderStage::Compute: return vk::ShaderStageFlagBits::eCompute;
       case gapi::ShaderStage::AllGraphics: return vk::ShaderStageFlagBits::eAllGraphics;
       case gapi::ShaderStage::All: return vk::ShaderStageFlagBits::eAll;
       case gapi::ShaderStage::Raygen: return vk::ShaderStageFlagBits::eRaygenKHR;
       case gapi::ShaderStage::AnyHit: return vk::ShaderStageFlagBits::eAnyHitKHR;
       case gapi::ShaderStage::ClosestHit: return vk::ShaderStageFlagBits::eClosestHitKHR;
       case gapi::ShaderStage::Miss: return vk::ShaderStageFlagBits::eMissKHR;
       case gapi::ShaderStage::Intersection: return vk::ShaderStageFlagBits::eIntersectionKHR;
       case gapi::ShaderStage::Callable: return vk::ShaderStageFlagBits::eCallableKHR;
       case gapi::ShaderStage::Task: return vk::ShaderStageFlagBits::eTaskNV;
       case gapi::ShaderStage::Mesh: return vk::ShaderStageFlagBits::eMeshNV;

       default:
       {
        ASSERT(!"unsupported");
        return {};
       }
    }
  }

  static vk::Format get_attribute_format(const gapi::AttributeType type)
  {
    switch (type)
    {
       case gapi::AttributeType::Float:  return vk::Format::eR32Sfloat;
       case gapi::AttributeType::Float2: return vk::Format::eR32G32Sfloat;
       case gapi::AttributeType::Float3: return vk::Format::eR32G32B32Sfloat;
       case gapi::AttributeType::Float4: return vk::Format::eR32G32B32A32Sfloat;
       case gapi::AttributeType::Int:    return vk::Format::eR32Sint;
       case gapi::AttributeType::Int2:   return vk::Format::eR32G32Sint;
       case gapi::AttributeType::Int3:   return vk::Format::eR32G32B32Sint;
       case gapi::AttributeType::Int4:   return vk::Format::eR32G32B32A32Sint;
       default:
       {
        ASSERT(!"unsupported");
        return {};
       }
    }
  }
}