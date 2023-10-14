#include "render.h"
#include "line_render.h"

#include <engine/ecs/macros.h>

#include <EASTL/vector.h>

ECS_DESCRIBE_QUERY(query_line_renderer, (Engine::dbg::LineRenderer& dbg_line_renderer))

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec)
  {
    query_line_renderer([&](auto& dbg_line_renderer){
      dbg_line_renderer.addLine(start, end, color, lifetime_sec);
    });
  }
}
