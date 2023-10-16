#pragma once

#include <engine/types.h>

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec);

  void draw_poly(const float3& a, const float3& b, const float3& c,
                 const float4& color, float lifetime_sec);

  void draw_aabb(const float3& center, const float3& extent, const float4& color, float lifetime_sec);
}
