#include "math.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

namespace math
{
  auto radians(float degree) -> float
  {
    return degree * float(M_PI) / 180.0f;
  }

  auto radians(const float2 degree) -> float2
  {
    return {
      radians(degree.x),
      radians(degree.y)
    };
  }

  auto perspective(float fov, float aspect, float zNear, float zFar) -> float4x4
  {
    const float tanA = std::tanf(radians(fov) / 2.0);

    glm::mat4 m(0);
    m[0][0] = 1 / (aspect * tanA);
    m[1][1] = 1 / tanA;
    m[2][2] = zFar / (zFar - zNear);
    m[2][3] = 1.0f;
    m[3][2] = -(zFar * zNear) / (zFar - zNear);

    return m;
  }

  auto perspective_inv_z(float fov, float aspect, float zNear, float zFar) -> float4x4
  {
    return perspective(fov, aspect, zFar, zNear);
  }

  auto look_at(const glm::vec3& at, const glm::vec3& from) -> float4x4
  {
    const glm::vec3 z = glm::normalize(at - from);
    const glm::vec3 x = glm::normalize(glm::cross(float3{0.0, 1.0, 0.0}, z));
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

  auto get_right(const float radians) -> float3
  {
    return glm::rotate(radians, float3{0.0, 1.0, 0.0}) * float4{1.0, 0.0, 0.0, 0.0};
  }

  auto rotate(const float roll, const float pitch, const float yaw) -> float4x4
  {
    float cosa = std::cos(-roll);
    float sina = std::sin(-roll);
    float cosb = std::cos(-yaw);
    float sinb = std::sin(-yaw);
    float cosg = std::cos(-pitch);
    float sing = std::sin(-pitch);

    float4x4 ZXY;
    ZXY[0] = float4{cosa*cosb-sina*sinb*sing, cosa*sinb*sing + sina*cosb, -sinb * cosg, 0};
    ZXY[1] = float4{-sina*cosg, cosa*cosg, sing, 0};
    ZXY[2] = float4{sina*cosb*sing + cosa*sinb, sina*sinb - cosa*cosb*sing, cosb * cosg, 0};
    ZXY[3] = float4{0,0,0,1};

    return ZXY;
  }

  auto rotateX(const float radians) -> float4x4
  {
    const float c = std::cos(-radians);
    const float s = std::sin(-radians);

    float4x4 X;
    X[0] = float4{1,0,0,0};
    X[1] = float4{0,c,s,0};
    X[2] = float4{0,-s,c,0};
    X[3] = float4{0,0,0,1};

    return X;
  }

  auto rotateY(const float radians) -> float4x4
  {
    const float c = std::cos(-radians);
    const float s = std::sin(-radians);

    float4x4 Y;
    Y[0] = float4{c,0,-s,0};
    Y[1] = float4{0,1,0,0};
    Y[2] = float4{s,0,c,0};
    Y[3] = float4{0,0,0,1};

    return Y;
  }

  auto rotateZ(const float radians) -> float4x4
  {
    const float c = std::cos(-radians);
    const float s = std::sin(-radians);

    float4x4 Z;
    Z[0] = float4{c,s,0,0};
    Z[1] = float4{-s,c,0,0};
    Z[2] = float4{0,0,1,0};
    Z[3] = float4{0,0,0,1};

    return Z;
  }

  static
  auto get_far_plane_points(const float4x4& proj, const float4x4& view) -> FarPlanePoints
  {
    const float4x4 invViewRotProj = glm::inverse(proj * view);

    float4 leftTopView =  invViewRotProj * float4(-1.0f, 1.0f, 0.0f, 1.0f);
    float4 rightTopView = invViewRotProj * float4( 1.0f, 1.0f, 0.0f, 1.0f);
    float4 leftBotView =  invViewRotProj * float4(-1.0f,-1.0f, 0.0f, 1.0f);
    float4 rightBotView = invViewRotProj * float4( 1.0f,-1.0f, 0.0f, 1.0f);
    leftTopView  /= leftTopView.w;
    rightTopView /= rightTopView.w;
    leftBotView  /= leftBotView.w;
    rightBotView /= rightBotView.w;

    return {
      .leftTop = leftTopView,
      .rightTop = rightTopView,
      .leftBot = leftBotView,
      .rightBot = rightBotView
    };
  }

  auto get_far_plane_points_world_space(const float4x4& proj, const float4x4& view) -> FarPlanePoints
  {
    return get_far_plane_points(proj, view);
  }

  auto get_far_plane_points_view_space(const float4x4& proj) -> FarPlanePoints
  {
    return get_far_plane_points(proj, float4x4{1});
  }
}