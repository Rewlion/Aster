#pragma once

#include <engine/types.h>

namespace Utils
{
  struct Basis
  {
    float3 e1, e2, e3;
  };

  struct Plane
  {
      Plane(const float3& pos, const float3& unit_normal);
      Basis getBasis() const;

      //Ax+By+Cz-D=0
      float3 N = float3(0.0, 1.0, 0.0);
      float D = 0.0;
  };
}