#include "math.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Math
{
  glm::mat4 Perspective(float fov, float aspect, float zNear, float zFar)
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

  glm::mat4 LookAt(const glm::vec3& at, const glm::vec3& from, const glm::vec3& up)
  {
    const glm::vec3 z = glm::normalize(at - from);
    const glm::vec3 x = glm::normalize(glm::cross(up, z));
    const glm::vec3 y = glm::normalize(glm::cross(z, x));

    const glm::mat4 M{
      glm::vec4{x[0],y[0],z[0],0},
      glm::vec4{x[1],y[1],z[1],0},
      glm::vec4{x[2],y[2],z[2],0},
      glm::vec4{
        -x[0] * from[0] - x[1] * from[1] - x[2] * from[2],
        -y[0] * from[0] - y[1] * from[1] - y[2] * from[2],
        -z[0] * from[0] - z[1] * from[1] - z[2] * from[2],
        1.0f,
      }
    };

    return M;
  }

  float Radians(float degree)
  {
    return degree * M_PI / 180.0f;
  }
}