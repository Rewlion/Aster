#include <engine/ecs/macros.h>
#include <engine/math.h>
#include <engine/types.h>

#include <EASTL/functional.h>
#include <glm/gtc/matrix_transform.hpp>

ECS_QUERY()
static void query_camera(eastl::function<void(
  const float3& pos,
  const float& camera_fov,
  const float& camera_zNear,
  const float& camera_zFar)> cb);

namespace Engine
{
  mat4 get_camera_vp()
  {
    float3 pos;
    float fov = 0;
    float zNear = 0;
    float zFar = 0;

    query_camera([&](const float3& _pos, const float& _fov, const float& _zNear, const float& _zFar){
      pos = _pos;
      fov = _fov;
      zNear = _zNear;
      zFar = _zFar;
    });

    mat4 vp = mat4{1};
    vp = glm::translate(vp, -pos);
    vp = math::perspective(fov, 3.0f/4.0f, zNear, zFar) * vp;

    return vp;
  }
}
