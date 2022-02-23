#include "math.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace math
{
  float Radians(float degree)
  {
    return degree * float(M_PI) / 180.0f;
  }

  mat4 Perspective(float fov, float aspect, float zNear, float zFar)
  {
    const float tanA = std::tanf(Radians(fov) / 2.0);

    glm::mat4 m(0);
    m[0][0] = 1 / (aspect * tanA);
    m[1][1] = 1 / tanA;
    m[2][2] = zFar / (zFar - zNear);
    m[2][3] = 1.0f;
    m[3][2] = -(zFar * zNear) / (zFar - zNear);

    return m;
  }
}