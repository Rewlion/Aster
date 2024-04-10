#include "camera.h"

#include <engine/ecs/macros.h>
#include <engine/math.h>
#include <engine/types.h>
#include <engine/render/world_render.h>

#include <EASTL/functional.h>
#include <glm/gtc/matrix_transform.hpp>

ECS_QUERY()
static void query_camera(eastl::function<void(
  const float3& pos,
  const float3& forward,
  const float& camera_fov,
  const float& camera_zNear,
  const float& camera_zFar)> cb);

ECS_QUERY()
static void query_camera_pos(eastl::function<void(
  const float3& pos,
  const float& camera_fov)> cb);

namespace Engine
{
  auto get_camera_pos() -> float3
  {
    float3 pos;
    query_camera_pos([&pos](auto _pos, auto unused)
    {
      pos = _pos;
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

    query_camera([&](const float3& _pos, const float3& _forward, const float& _fov, const float& _zNear, const float& _zFar){
      pos = _pos;
      forward = _forward;
      fov = _fov;
      zNear = _zNear;
      zFar = _zFar;
    });

    std::swap(zNear, zFar);

    float4x4 proj = math::perspective(fov, aspect, zNear, zFar);
    proj[2][0] = jitter.x;
    proj[2][1] = jitter.y;

    return {
      .pos = pos,
      .view = math::look_at(pos + forward * 2.0f, pos),
      .proj = proj,
      .zNear = zNear,
      .zFar = zFar
    };
  }
}
