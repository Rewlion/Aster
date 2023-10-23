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

  auto get_far_plane_points_view_space(const float4x4& proj, const float4x4& view) -> FarPlanePoints
  {
    return get_far_plane_points(proj, float4x4{1});
  }
}