#pragma once

#include <engine/types.h>

namespace Utils
{
  struct Plane;
  struct Sphere;
  struct Frustum;
}

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec);

  void draw_poly(const float3& a, const float3& b, const float3& c,
                 const float4& color, float lifetime_sec);
  void draw_poly(const float3* triangles, const float4* colors,
                 const size_t count, const float lifetime_sec);
  void draw_aabb(const float3& center, const float3& extent, const float4& color, const float lifetime_sec);
  void draw_line_plane(const Utils::Plane&, const float3& pos, const float size,
                       const float3& color, const float lifetime_sec, const bool draw_basis=false);
  void draw_plane(const Utils::Plane&, const float3& pos, const float size,
                  const float4& color, const float lifetime_sec, const bool draw_basis=false);
  void draw_line_sphere(const Utils::Sphere&, const float3& color, const float lifetime_sec);
  void draw_line_sphere(const float3& pos, const float r, const float3& color, const float lifetime_sec);
  void draw_frustum(const Utils::Frustum&, const float4& color, const float lifetime_sec, const bool draw_basis=false);
  void draw_frustums(const Utils::Frustum*, const float4* colors, const size_t count, const float lifetime_sec);
}
