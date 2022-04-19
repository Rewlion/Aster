#pragma once

#include <engine/types.h>

namespace math
{
  float radians(float degree);
  mat4 perspective(float fov, float aspect, float zNear, float zFar);
  glm::mat4 look_at(const glm::vec3& at, const glm::vec3& from);
  float3 get_right(const float radians);
}
