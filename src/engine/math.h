#pragma once

#include <engine/types.h>

namespace math
{
  auto radians(float degree) -> float;
  auto perspective(float fov, float aspect, float zNear, float zFar) -> float4x4;
  auto perspective_inv_z(float fov, float aspect, float zNear, float zFar) -> float4x4;
  auto look_at(const glm::vec3& at, const glm::vec3& from) -> float4x4;
  auto get_right(const float radians) -> float3;

  struct FarPlanePoints
  {
    float3 leftTop, rightTop, leftBot, rightBot;
  };
  auto get_far_plane_points_world_space(const float4x4& proj, const float4x4& view) -> FarPlanePoints;
  auto get_far_plane_points_view_space(const float4x4& proj) -> FarPlanePoints;

  inline
  auto spherical_to_decartian(float theta, float phi) -> float3
  {
    return float3(
      std::sin(theta)*std::sin(phi),
      std::cos(theta),
      std::sin(theta)*std::cos(phi)
    );
  }

  template <typename T>
  inline
  auto sign(T val) -> int
  {
    return (T(0) < val) - (val < T(0));
  }
}
