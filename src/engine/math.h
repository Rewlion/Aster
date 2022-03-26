#pragma once

#include <engine/types.h>

namespace math
{
  float radians(float degree);
  mat4 perspective(float fov, float aspect, float zNear, float zFar);
}
