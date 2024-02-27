#pragma once

#include <engine/types.h>

#include <optional>

#include <EASTL/tuple.h>

namespace Utils
{
  struct Basis
  {
    float3 e1, e2, e3;
  };

  struct Plane
  {
    Plane() = default;
    Plane(const float3& unit_normal, const float D);
    Plane(const float3& pos, const float3& unit_normal);
    auto getBasis() const -> Basis;
    auto inv() const -> Plane;

    //Ax+By+Cz-D=0
    float3 N = float3(0.0, 1.0, 0.0);
    float D = 0.0;
  };

  struct Sphere
  {
    float3 center;
    float r;
  };

  struct AABB
  {
    float3 min,max;
  };

  struct Ray
  {
    float3 pos;
    float3 dir;
  };

  inline
  auto operator*(const float4x4& tm, const Ray& ray) -> Ray
  {
    const float3 pos = tm * float4(ray.pos, 1.0f);
    const float3 dir = tm * float4(ray.dir, 0.0f);
    return Ray{pos,dir};
  }

  struct Triangle
  {
    float3 a,b,c;
  };
  struct Frustum
  {
    enum PlaneType{LEFT,RIGHT,BOT,TOP,NEAR,FAR, LAST_PLANE};
    
    Frustum() = default;
    Frustum(const float4x4& view_projection);
    Frustum(const Plane& left, const Plane& right, const Plane& bot,
            const Plane& top, const Plane& near, const Plane& far)
    {
      planes[LEFT] = left;
      planes[RIGHT] = right;
      planes[BOT] = bot;
      planes[TOP] = top;
      planes[NEAR] = near;
      planes[FAR] = far;
    }

    Plane planes[6];
  };

  auto calc_sign_distance(const Plane&, const float3&) -> float;
  auto calc_abs_distance(const Plane&, const float3&) -> float;
  auto calc_abs_distance(const Sphere&, const Sphere&) -> float;
  auto calc_abs_distance_squared(const Sphere&, const Sphere&) -> float;

  auto test_half_space(const Plane&, const Sphere&) -> bool;

  auto test_intersection(const Plane&, const Sphere&) -> bool;
  auto test_intersection(const Sphere&, const Sphere&) -> bool;
  auto test_intersection(const Frustum&, const Sphere&) -> bool;
  auto test_intersection(const AABB&, const Sphere&) -> bool;
  auto test_intersection(const AABB&, const Ray&) -> bool;

  auto calc_intersect_point(const Plane& p1, const Plane& p2, const Plane& p3) -> std::optional<float3>;
  auto calc_intersection_t(const AABB&, const Ray&) -> float;
  auto calc_intersection_t(const Triangle&, const Ray&) -> eastl::tuple<float, float2>;

  auto calc_closest_point(const AABB&, const Sphere&) -> float3;

}