#include "render.h"
#include "line_render.h"
#include "poly_render.h"

#include <engine/ecs/macros.h>
#include <engine/math.h>
#include <engine/utils/collision.h>

#include <EASTL/vector.h>
#include <numbers>

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

  void draw_poly(const float3* triangles, const float4* colors,
                 const size_t count, const float lifetime_sec)
  {
    query_poly_renderer([&](auto& renderer){
      renderer.addPoly(triangles, colors, count, lifetime_sec);
    });
  }

  void draw_aabb(const float3& center, const float3& extent, const float4& color, const float lifetime_sec)
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

  static
  void draw_line_basis(const float3& e1, const float3& e2, const float3& e3, const float3& pos, const float size, const float lifetime_sec)
  {
    draw_line(pos, pos + e1*size, float3(1.0, 0.0, 0.0), lifetime_sec);
    draw_line(pos, pos + e2*size, float3(0.0, 1.0, 0.0), lifetime_sec);
    draw_line(pos, pos + e3*size, float3(0.0, 0.0, 1.0), lifetime_sec);
  }

  void draw_line_plane(const Utils::Plane& plane, const float3& pos, const float size,
                       const float3& color, const float lifetime_sec, const bool draw_basis)
  {
    const auto[e1, e2, e3] = plane.getBasis();

    const float halfSize = size*0.5f;
    const float3 center = pos;

    const float3 a = center + (-e1-e3)*halfSize;
    const float3 b = center + (-e1+e3)*halfSize;
    const float3 c = center + (e1+e3)*halfSize;
    const float3 d = center + (e1-e3)*halfSize;

    draw_line(a, b, color, lifetime_sec);
    draw_line(b, c, color, lifetime_sec);
    draw_line(c, d, color, lifetime_sec);
    draw_line(d, a, color, lifetime_sec);

    if (draw_basis)
      draw_line_basis(e1,e2,e3, center, size*0.1f, lifetime_sec);
  }

  void draw_plane(const Utils::Plane& plane, const float3& pos, const float size,
                  const float4& color, const float lifetime_sec, const bool draw_basis)
  {
    const auto[e1, e2, e3] = plane.getBasis();

    const float halfSize = size*0.5f;
    const float3 center = pos;

    const float3 a = center + (-e1-e3)*halfSize;
    const float3 b = center + (-e1+e3)*halfSize;
    const float3 c = center + (e1+e3)*halfSize;
    const float3 d = center + (e1-e3)*halfSize;

    draw_poly(a,b,c, color, lifetime_sec);
    draw_poly(c,d,a, color, lifetime_sec);

    if (draw_basis)
      draw_line_basis(e1,e2,e3, center, size*0.1f, lifetime_sec);
  }

  void draw_line_sphere(const Utils::Sphere& sphere, const float3& color, const float lifetime_sec)
  {
    const size_t Ni = 9;
    const size_t Nj = Ni+2;

    const auto calcPhi = [Ni](const int i){ return 2.0 * std::numbers::pi_v<float> * (float)i / (float)Ni; };
    const auto calcTheta = [Nj](const int j){ return std::numbers::pi_v<float> * (float)j / (float)Nj; };

    for (size_t i = 0; i < Ni; ++i)
      for (size_t j = 0; j < Nj; ++j)
    {
      const float phi  = calcPhi(i);
      const float phi2  = calcPhi(i+1);

      const float theta  = calcTheta(j);
      const float theta2 = calcTheta(j+1);

      const float3 begin = sphere.center + math::spherical_to_decartian(theta, phi) * sphere.r;
      const float3 endVertical = sphere.center + math::spherical_to_decartian(theta2, phi) * sphere.r;
      const float3 endHorizontal = sphere.center + math::spherical_to_decartian(theta, phi2) * sphere.r;

      draw_line(begin, endVertical, color, lifetime_sec);
      draw_line(begin, endHorizontal, color, lifetime_sec);
    }
  }

  void draw_line_sphere(const float3& pos, const float r, const float3& color, const float lifetime_sec)
  {
    draw_line_sphere({pos, r}, color, lifetime_sec);
  }

  void draw_frustum(const Utils::Frustum& fr, const float4& color, const float lifetime_sec, const bool draw_basis)
  {
    //    f____g
    //  / |   /|
    // b-----c |
    // |  e_ | h
    // a-----d/
    using Utils::Frustum;
    const float3 a = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::BOT], fr.planes[Frustum::LEFT]).value();
    const float3 b = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::TOP], fr.planes[Frustum::LEFT]).value();
    const float3 c = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::TOP], fr.planes[Frustum::RIGHT]).value();
    const float3 d = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::BOT], fr.planes[Frustum::RIGHT]).value();

    const float3 e = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::BOT], fr.planes[Frustum::LEFT]).value();
    const float3 f = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::TOP], fr.planes[Frustum::LEFT]).value();
    const float3 g = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::TOP], fr.planes[Frustum::RIGHT]).value();
    const float3 h = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::BOT], fr.planes[Frustum::RIGHT]).value();

    draw_poly(a,b,c, color, lifetime_sec);
    draw_poly(c,d,a, color, lifetime_sec);

    draw_poly(e,f,g, color, lifetime_sec);
    draw_poly(g,h,e, color, lifetime_sec);

    draw_poly(b,f,g, color, lifetime_sec);
    draw_poly(g,c,b, color, lifetime_sec);

    draw_poly(d,c,g, color, lifetime_sec);
    draw_poly(g,h,d, color, lifetime_sec);

    draw_poly(a,b,f, color, lifetime_sec);
    draw_poly(f,e,a, color, lifetime_sec);

    draw_poly(a,e,h, color, lifetime_sec);
    draw_poly(h,d,a, color, lifetime_sec);

    if (draw_basis)
    {
      float3 centers[Frustum::LAST_PLANE];
      centers[Frustum::LEFT]  = (a+f)*0.5f;
      centers[Frustum::RIGHT] = (d+g)*0.5f;
      centers[Frustum::BOT]   = (e+d)*0.5f;
      centers[Frustum::TOP]   = (b+g)*0.5f;
      centers[Frustum::NEAR]  = (a+c)*0.5f;
      centers[Frustum::FAR]   = (e+g)*0.5f;
      for (int i = 0; i < Frustum::LAST_PLANE; ++i)
      {
        const auto[e1,e2,e3] = fr.planes[i].getBasis();
        draw_line_basis(e1,e2,e3, centers[i], 0.1f, lifetime_sec);
      }
    }
  }

  void draw_frustums(const Utils::Frustum* frustums, const float4* colors, const size_t count, const float lifetime_sec)
  {
    //    f____g
    //  / |   /|
    // b-----c |
    // |  e_ | h
    // a-----d/
    using Utils::Frustum;

    const size_t verticesPerFrustum = 6 * 6;
    const size_t colorsPerFrustum = 6 * 2;
  
    eastl::vector<float3> triVertices;
    eastl::vector<float4> triColors;
    triVertices.reserve(verticesPerFrustum * count);
    triColors.reserve(colorsPerFrustum * count);

    for (size_t iFrustum = 0; iFrustum < count ; ++iFrustum)
    {
      const Frustum& fr = frustums[iFrustum];
      const float4& color = colors[iFrustum];

      const float3 a = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::BOT], fr.planes[Frustum::LEFT]).value();
      const float3 b = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::TOP], fr.planes[Frustum::LEFT]).value();
      const float3 c = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::TOP], fr.planes[Frustum::RIGHT]).value();
      const float3 d = Utils::calc_intersect_point(fr.planes[Frustum::NEAR], fr.planes[Frustum::BOT], fr.planes[Frustum::RIGHT]).value();

      const float3 e = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::BOT], fr.planes[Frustum::LEFT]).value();
      const float3 f = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::TOP], fr.planes[Frustum::LEFT]).value();
      const float3 g = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::TOP], fr.planes[Frustum::RIGHT]).value();
      const float3 h = Utils::calc_intersect_point(fr.planes[Frustum::FAR], fr.planes[Frustum::BOT], fr.planes[Frustum::RIGHT]).value();

      #define ADD_POLY(a,b,c,color)\
        triVertices.push_back(a);\
        triVertices.push_back(b);\
        triVertices.push_back(c);\
        triColors.push_back(color)

        ADD_POLY(a,b,c, color);
        ADD_POLY(c,d,a, color);

        ADD_POLY(e,f,g, color);
        ADD_POLY(g,h,e, color);

        ADD_POLY(b,f,g, color);
        ADD_POLY(g,c,b, color);

        ADD_POLY(d,c,g, color);
        ADD_POLY(g,h,d, color);

        ADD_POLY(a,b,f, color);
        ADD_POLY(f,e,a, color);

        ADD_POLY(a,e,h, color);
        ADD_POLY(h,d,a, color);
      #undef ADD_POLY
    }

    draw_poly(triVertices.data(), triColors.data(), verticesPerFrustum / 3 * count, lifetime_sec);
  }
}
