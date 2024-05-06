#include "camera.h"

#include <engine/components/camera.h>
#include <engine/ecs/macros.h>
#include <engine/math.h>
#include <engine/types.h>
#include <engine/render/world_render.h>

#include <EASTL/functional.h>
#include <glm/gtc/matrix_transform.hpp>

ECS_DESCRIBE_QUERY(query_camera, const CameraComponent& camera);

namespace Engine
{
  auto get_camera_pos() -> float3
  {
    float3 pos;
    query_camera([&pos](const CameraComponent& camera)
    {
      if (camera.isActive())
        pos = camera.getWorldPosition();
    });

    return pos;
  }

  auto get_camera(const float aspect, const float3 jitter) -> CameraData
  {
    float3 pos;
    float3 forward;
    float fov = 0;
    float zNear = 0;
    float zFar = 0;

    query_camera([&](const CameraComponent& camera)
    {
      if (camera.isActive())
      {
        const float4x4 tm = camera.getWorldTransform();
        pos = tm[3];
        forward = tm[2];
        fov = camera.getFOV();
        zNear = camera.getZNear();
        zFar = camera.getZFar();
      }
    });

    const float nearPlaneDist = zNear;
    const float farPlaneDist = zFar;

    std::swap(zNear, zFar);

    float4x4 proj = math::perspective(fov, aspect, zNear, zFar);
    proj[2][0] = jitter.x;
    proj[2][1] = jitter.y;

    return {
      .pos = pos,
      .view = math::look_at(pos + forward * 2.0f, pos),
      .proj = proj,
      .zNear = zNear,
      .zFar = zFar,
      .nearPlaneDist = nearPlaneDist,
      .farPlaneDist = farPlaneDist
    };
  }
}
