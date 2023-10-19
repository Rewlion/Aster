#include "collision.h"

#include <engine/assert.h>

namespace Utils
{
  Plane::Plane(const float3& pos, const float3& unit_normal)
  {
    N = unit_normal;
    D = glm::dot(unit_normal, pos);
  }

  auto Plane::getBasis() const -> Basis
  {
    const float3 up = std::abs(N.y) != 1.0 ?
                        float3(0.0, 1.0, 0.0) :
                        float3(0.0, 0.0, 1.0);
    const float3 T = glm::cross(N, up);
    const float3 BT = glm::cross(T, N);
    return {
      .e1 = T,
      .e2 = N,
      .e3 = BT
    };
  }

  Frustum::Frustum(const float4x4& view_projection)
  {
    //https://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf

    planes[LEFT].N = {
      view_projection[0][3] + view_projection[0][0],
      view_projection[1][3] + view_projection[1][0],
      view_projection[2][3] + view_projection[2][0]
    };
    planes[LEFT].D = -(view_projection[3][3] + view_projection[3][0]);

    planes[RIGHT].N = {
      view_projection[0][3] - view_projection[0][0],
      view_projection[1][3] - view_projection[1][0],
      view_projection[2][3] - view_projection[2][0]
    };
    planes[RIGHT].D = -(view_projection[3][3] - view_projection[3][0]);

    planes[BOT].N = {
      view_projection[0][3] + view_projection[0][1],
      view_projection[1][3] + view_projection[1][1],
      view_projection[2][3] + view_projection[2][1]
    };
    planes[BOT].D = -(view_projection[3][3] + view_projection[3][1]);

    planes[TOP].N = {
      view_projection[0][3] - view_projection[0][1],
      view_projection[1][3] - view_projection[1][1],
      view_projection[2][3] - view_projection[2][1]
    };
    planes[TOP].D = -(view_projection[3][3] - view_projection[3][1]);

    planes[NEAR].N = {
      view_projection[0][2],
      view_projection[1][2],
      view_projection[2][2],
    };
    planes[NEAR].D = -view_projection[3][2];

    planes[FAR].N = {
      view_projection[0][3] - view_projection[0][2],
      view_projection[1][3] - view_projection[1][2],
      view_projection[2][3] - view_projection[2][2],
    };
    planes[FAR].D = -(view_projection[3][3] - view_projection[3][2]);
  }

  auto calc_sign_distance(const Plane& pl, const float3& p) -> float
  {
    return glm::dot(p, pl.N) - pl.D;
  }

  auto calc_abs_distance(const Plane& pl, const float3& p) -> float
  {
    return std::abs(calc_sign_distance(pl, p));
  }

  auto calc_abs_distance(const Sphere& s1, const Sphere& s2) -> float
  {
    return std::sqrt(calc_abs_distance_squared(s1, s2));
  }

  auto calc_abs_distance_squared(const Sphere& s1, const Sphere& s2) -> float
  {
    const float3 dl = s2.center - s1.center;
    return std::abs(glm::dot(dl, dl));
  }

  auto test_intersection(const Plane& pl, const Sphere& s) -> bool
  {
    return calc_abs_distance(pl, s.center) <= s.r;
  }

  auto test_intersection(const Sphere& s1, const Sphere& s2) -> bool
  {
    const float r1r2 = s1.r + s2.r;
    return calc_abs_distance_squared(s1, s2) <= (r1r2*r1r2);
  }

  auto calc_intersect_point(const Plane& p1, const Plane& p2, const Plane& p3) -> std::optional<float3>
  {
    const float3 c1{p1.N.x, p2.N.x, p3.N.x};
    const float3 c2{p1.N.y, p2.N.y, p3.N.y};
    const float3 c3{p1.N.z, p2.N.z, p3.N.z};
    const float3 c4{p1.D,   p2.D,   p3.D};

    float3 u = glm::cross(c2,c3);
    float3 v = glm::cross(c1,c4);
    
    const float dMain = glm::dot(c1, u);
    if (std::abs(dMain) < 0.0001)
      return std::nullopt;

    const float d1 = glm::dot(c4,u);
    const float d2 = glm::dot(c3,v);
    const float d3 = -glm::dot(c2,v);

    return float3{d1, d2, d3} / dMain;
  }
}
