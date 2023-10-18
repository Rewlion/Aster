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
}
