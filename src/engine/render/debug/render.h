#pragma once

#include <engine/types.h>

namespace Utils
{
  struct Plane;
}

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec);

  void draw_poly(const float3& a, const float3& b, const float3& c,
                 const float4& color, float lifetime_sec);

  void draw_aabb(const float3& center, const float3& extent, const float4& color, const float lifetime_sec);
  void draw_line_plane(const Utils::Plane&, const float3& pos, const float size,
                       const float3& color, const float lifetime_sec, const bool draw_basis=false);
  void draw_plane(const Utils::Plane&, const float3& pos, const float size,
                  const float4& color, const float lifetime_sec, const bool draw_basis=false);
}
