#pragma once

#include <engine/types.h>

#include <optional>

namespace Utils
{
  struct Basis
  {
    float3 e1, e2, e3;
  };

  struct Plane
  {
      Plane(const float3& pos, const float3& unit_normal);
      auto getBasis() const -> Basis;

      //Ax+By+Cz-D=0
      float3 N = float3(0.0, 1.0, 0.0);
      float D = 0.0;
  };

  struct Sphere
  {
    float3 center;
    float r;
  };

  auto calc_sign_distance(const Plane&, const float3&) -> float;
  auto calc_abs_distance(const Plane&, const float3&) -> float;
  auto calc_abs_distance(const Sphere&, const Sphere&) -> float;
  auto calc_abs_distance_squared(const Sphere&, const Sphere&) -> float;

  auto test_intersection(const Plane&, const Sphere&) -> bool;
  auto test_intersection(const Sphere&, const Sphere&) -> bool;
  auto calc_intersect_point(const Plane& p1, const Plane& p2, const Plane& p3) -> std::optional<float3>;

}