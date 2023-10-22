#pragma once

#include <engine/types.h>

namespace math
{
  float radians(float degree);
  mat4 perspective(float fov, float aspect, float zNear, float zFar);
  mat4 perspective_inv_z(float fov, float aspect, float zNear, float zFar);
  glm::mat4 look_at(const glm::vec3& at, const glm::vec3& from);
  float3 get_right(const float radians);

  struct FarPlanePoints
  {
    float3 leftTop, rightTop, leftBot, rightBot;
  };

  auto get_far_plane_points_world_space(const float4x4& proj, const float4x4& view) -> FarPlanePoints;
  auto get_far_plane_points_view_space(const float4x4& proj, const float4x4& view) -> FarPlanePoints;
}
