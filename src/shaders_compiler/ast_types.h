#pragma once

#include <engine/assert.h>

#include <optional>
#include <stdint.h>

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace ShadersSystem
{
  enum class ResourceType: uint8_t
  {
    None = 0,
    Cbuffer = 1,
    Texture2D = 2,
    CbufferVar = 3
  };

  enum class AttributeType: uint8_t
  {
    None = 0,
    Float = 3,
    Float2 = 4,
    Float3 = 5,
    Float4 = 6,
    Int = 7,
    Int2 = 8,
    Int3 = 9,
    Int4 = 10,
  };

  inline size_t attributeType_to_size(const AttributeType type)
  {
    switch (type)
    {
      case AttributeType::Float:
      case AttributeType::Int:
        return 4;
      case AttributeType::Float2:
      case AttributeType::Int2:
        return 8;
      case AttributeType::Float3:
      case AttributeType::Int3:
        return 12;
      case AttributeType::Float4:
      case AttributeType::Int4:
        return 16;

      default:
      {
        ASSERT(!"unsupported type");
        return 0;
      }
    }
  }

  inline string attributeType_to_string(const AttributeType type)
  {
    switch (type)
    {
      case AttributeType::Float:  return "float";
      case AttributeType::Int:    return "int";
      case AttributeType::Float2: return "float2";
      case AttributeType::Int2:   return "int2";
      case AttributeType::Float3: return "float3";
      case AttributeType::Int3:   return "int3";
      case AttributeType::Float4: return "float4";
      case AttributeType::Int4:   return "int4";

      default:
      {
        ASSERT(!"unsupported type");
        return 0;
      }
    }
  }

  enum class ResourceAccessType: uint8_t
  {
    None = 0,
    Extern = 1,
    Channel = 2
  };

  enum class TargetProfile
  {
    None,
    VS_6_0, PS_6_0,
    VS_6_1, PS_6_1,
    VS_6_2, PS_6_2,
    VS_6_3, PS_6_3,
    VS_6_4, PS_6_4,
    VS_6_5, PS_6_5,
    VS_6_6, PS_6_6,
    VS_6_7, PS_6_7,
  };
}
