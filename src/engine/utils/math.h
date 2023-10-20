#pragma once

#include <engine/types.h>

namespace Utils
{
  inline
  auto spherical_to_decartian(float theta, float phi) -> float3
  {
    return float3(
      std::sin(theta)*std::sin(phi),
      std::cos(theta),
      std::sin(theta)*std::cos(phi)
    );
  }

  template <typename T>
  inline
  auto sign(T val) -> int
  {
    return (T(0) < val) - (val < T(0));
  }
}
