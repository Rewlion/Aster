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
    None,
    Cbuffer,
    Texture2D,
    Texture3D,
    TextureCube,
    RWStructuredBuffer,
    RWBuffer,
    RWTexture3D,
    CbufferVar,
    Sampler
  };

  inline
  auto to_hlsl_type(const ResourceType type) -> const char*
  {
    switch (type)
    {
      case ResourceType::Texture2D :          return "Texture2D";
      case ResourceType::Texture3D :          return "Texture3D";
      case ResourceType::TextureCube :        return "TextureCube";
      case ResourceType::RWStructuredBuffer : return "RWStructuredBuffer";
      case ResourceType::RWBuffer :           return "RWBuffer";
      case ResourceType::RWTexture3D :        return "RWTexture3D";
      case ResourceType::Sampler :            return "sampler";
      default: ASSERT_FMT_RETURN(false, "", "unsupported {}", (int)type);
    }
  }

  inline
  auto to_hlsl_register(const ResourceType type) -> const char*
  {
    switch (type)
    {
      case ResourceType::Texture2D :
      case ResourceType::Texture3D :
      case ResourceType::TextureCube :        return "t";
      case ResourceType::RWStructuredBuffer :
      case ResourceType::RWBuffer :
      case ResourceType::RWTexture3D :        return "u";
      case ResourceType::Sampler :            return "s";
      default: ASSERT_FMT_RETURN(false, "", "unsupported {}", (int)type);
    }
  }

  enum class ResourceAccessType: uint8_t
  {
    None = 0,
    Extern = 1,
    Channel = 2
  };
}
