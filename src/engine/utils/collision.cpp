#include "collision.h"

namespace Utils
{
  Plane::Plane(const float3& pos, const float3& unit_normal)
  {
    N = unit_normal;
    D = glm::dot(unit_normal, pos);
  }

  Basis Plane::getBasis() const
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
}
