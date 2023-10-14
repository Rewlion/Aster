#pragma once

#include <engine/types.h>

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec);
}
