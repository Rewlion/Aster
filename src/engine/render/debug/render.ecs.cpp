#include "render.h"
#include "line_render.h"
#include "poly_render.h"

#include <engine/ecs/macros.h>

#include <EASTL/vector.h>

ECS_DESCRIBE_QUERY(query_line_renderer, (Engine::dbg::LineRenderer& dbg_line_renderer))
ECS_DESCRIBE_QUERY(query_poly_renderer, (Engine::dbg::PolyRenderer& dbg_poly_renderer))

namespace Engine::dbg
{
  void draw_line(const float3& start, const float3& end,
                 const float3& color, float lifetime_sec)
  {
    query_line_renderer([&](auto& renderer){
      renderer.addLine(start, end, color, lifetime_sec);
    });
  }

  void draw_poly(const float3& a, const float3& b, const float3& c,
                 const float4& color, float lifetime_sec)
  {
    query_poly_renderer([&](auto& renderer){
      renderer.addPoly(a, b, c, color, lifetime_sec);
    });
  }

  void draw_aabb(const float3& center, const float3& extent, const float4& color, float lifetime_sec)
  {
    const float3 halfExtent = extent * 0.5f;

    struct Plane
    {
      float3 a,b,c,d;
    };

    Plane planes[] {
      {
        float3{-1, -1, -1},
        float3{-1,1,-1},
        float3{1,-1,-1},
        float3{1,1,-1}
      },
      {
        float3{-1, -1, 1},
        float3{-1, 1, 1},
        float3{1, -1, 1},
        float3{1, 1, 1},
      },
      {
        float3{1, -1, -1},
        float3{1, 1, -1},
        float3{1, -1, 1},
        float3{1, 1, 1},
      },
      {
        float3{-1, -1, -1},
        float3{-1, 1, -1},
        float3{-1, -1, 1},
        float3{-1, 1, 1},
      },
      {
        float3{-1, 1, -1},
        float3{-1, 1, 1},
        float3{1, 1, -1},
        float3{1, 1, 1},
      },
      {
        float3{-1, -1, -1},
        float3{-1, -1, 1},
        float3{1, -1, -1},
        float3{1, -1, 1},
      },
    };

    const size_t nPlanes = std::size(planes);
    for (size_t i = 0; i < nPlanes; ++i)
    {
      const Plane& p = planes[i];
      const float3 a = p.a * halfExtent + center;
      const float3 b = p.b * halfExtent + center;
      const float3 c = p.c * halfExtent + center;
      const float3 d = p.d * halfExtent + center;

      draw_poly(a,b,c, color, lifetime_sec);
      draw_poly(c,b,d, color, lifetime_sec);
    }
  }
}
