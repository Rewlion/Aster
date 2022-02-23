#pragma once

#include <engine/types.h>

namespace math
{
  float Radians(float degree);
  mat4 Perspective(float fov, float aspect, float zNear, float zFar);
}
