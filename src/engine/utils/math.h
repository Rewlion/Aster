#pragma once

#include <engine/types.h>

namespace Utils
{
  float3 spherical_to_decartian(float theta, float phi)
  {
    return float3(
      std::sin(theta)*std::sin(phi),
      std::cos(theta),
      std::sin(theta)*std::cos(phi)
    );
  }
}
