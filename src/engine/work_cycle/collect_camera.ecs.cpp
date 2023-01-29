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
  CameraData get_camera()
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

    mat4 vp = mat4{1};
    vp = math::perspective(fov, Engine::Render::world_render.getWndAspect(), zNear, zFar) *
         math::look_at(pos + forward * 2.0f, pos);

    return {
      .pos = pos,
      .viewProj = vp
    };
  }
}
