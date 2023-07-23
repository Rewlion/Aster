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

  enum class ResourceAccessType: uint8_t
  {
    None = 0,
    Extern = 1,
    Channel = 2
  };

  enum class TargetProfile
  {
    None,
    VS_6_0, PS_6_0, CS_6_0,
    VS_6_1, PS_6_1, CS_6_1,
    VS_6_2, PS_6_2, CS_6_2,
    VS_6_3, PS_6_3, CS_6_3,
    VS_6_4, PS_6_4, CS_6_4,
    VS_6_5, PS_6_5, CS_6_5,
    VS_6_6, PS_6_6, CS_6_6,
    VS_6_7, PS_6_7, CS_6_7,
  };
}
