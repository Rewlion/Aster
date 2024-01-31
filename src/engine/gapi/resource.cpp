#include "resources.h"

#include <engine/assert.h>

#include <boost/functional/hash.hpp>

namespace gapi
{
  size_t DepthStencilStateDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, depthTestEnabled);
    hash_combine(hash, depthWriteEnabled);
    hash_combine(hash, depthOp);

    hash_combine(hash, stencilTestEnabled);
    hash_combine(hash, stencilFailOp);
    hash_combine(hash, stencilPassOp);
    hash_combine(hash, stencilDepthFailOp);
    hash_combine(hash, stencilCompareOp);
    hash_combine(hash, stencilReferenceValue);

    return hash;
  }

  size_t VertexInputDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    for (const auto& attr: attributes)
    {
      hash_combine(hash, attr.offset);
      hash_combine(hash, attr.location);
      hash_combine(hash, attr.binding);
      hash_combine(hash, attr.type);
    }

    for (const auto& buf: buffers)
    {
      hash_combine(hash, buf.stride);
      hash_combine(hash, buf.binding);
    }

    return hash;
  }

  size_t GraphicsPipelineDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, layout);
    for(const auto handler: shaders)
      hash_combine(hash, handler);
    hash_combine(hash, ia.hash());
    hash_combine(hash, cullMode);
    hash_combine(hash, topology);
    hash_combine(hash, depthStencilState.hash());
    hash_combine(hash, blendState.hash());
    hash_combine(hash, tsInputControlPatchCount);

    return hash;
  }

  size_t ComputePipelineDescription::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, layout);
    hash_combine(hash, shader);
    hash_combine(hash, numthreads.x);
    hash_combine(hash, numthreads.y);
    hash_combine(hash, numthreads.z);

    return hash;
  }

  size_t AttachmentBlendState::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

     hash_combine(hash, blendEnabled);
     hash_combine(hash, srcColorBlendFactor);
     hash_combine(hash, dstColorBlendFactor);
     hash_combine(hash, colorBlendOp);
     hash_combine(hash, srcAlphaBlendFactor);
     hash_combine(hash, dstAlphaBlendFactor);
     hash_combine(hash, alphaBlendOp);

    return hash;
  }

  size_t BlendState::hash() const
  {
    using boost::hash_combine;
    size_t hash = 0;

    hash_combine(hash, logicOpEnabled);
    hash_combine(hash, logicOp);
    hash_combine(hash, blendConstants[0]);
    hash_combine(hash, blendConstants[1]);
    hash_combine(hash, blendConstants[2]);
    hash_combine(hash, blendConstants[3]);

    for (const auto& attachment: attachments)
      hash_combine(hash, attachment.hash());

    return hash;
  }

  auto attributeType_to_size(const AttributeType type) -> size_t
  {
    switch (type)
    {
      case AttributeType::Float4_u8:
      case AttributeType::Float:
      case AttributeType::UInt:
      case AttributeType::Int:
        return 4;
      case AttributeType::Float2:
      case AttributeType::UInt2:
      case AttributeType::Int2:
        return 8;
      case AttributeType::Float3:
      case AttributeType::UInt3:
      case AttributeType::Int3:
        return 12;
      case AttributeType::Float4:
      case AttributeType::UInt4:
      case AttributeType::Int4:
        return 16;
      case AttributeType::Float4x4:
        return 16 * 4;

      default:
      {
        ASSERT(!"unsupported type");
        return 0;
      }
    }
  }

  auto attributeType_to_string(const AttributeType type) -> const char*
  {
    switch (type)
    {
      case AttributeType::Float:        return "float";
      case AttributeType::UInt:         return "uint";
      case AttributeType::Int:          return "int";
      case AttributeType::Float2:       return "float2";
      case AttributeType::UInt2:        return "uint2";
      case AttributeType::Int2:         return "int2";
      case AttributeType::Float3:       return "float3";
      case AttributeType::UInt3:        return "uint3";
      case AttributeType::Int3:         return "int3";
      case AttributeType::Float4:       return "float4";
      case AttributeType::Float4_u8:    return "float4";
      case AttributeType::UInt4:        return "uint4";
      case AttributeType::Int4:         return "int4";
      case AttributeType::Float4x4:     return "float4x4";

      default:
      {
        ASSERT(!"unsupported type");
        return "";
      }
    }
  }
}
