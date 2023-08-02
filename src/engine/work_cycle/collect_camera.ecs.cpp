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

namespace Engine
{
  CameraData get_camera(const float aspect, const float3 jitter)
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

    pos += jitter;

    std::swap(zNear, zFar);

    return {
      .pos = pos,
      .view = math::look_at(pos + forward * 2.0f, pos),
      .proj = math::perspective(fov, aspect, zNear, zFar),
      .zNear = zNear,
      .zFar = zFar
    };
  }
}
