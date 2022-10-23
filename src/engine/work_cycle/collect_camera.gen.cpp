// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/registry.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "collect_camera.ecs.cpp" 

using namespace Engine::ECS;

const static query_id query_camera_queryId = Registry::registerDirectQuery(
  DirectQueryDescription{
    .components = {
      DESCRIBE_QUERY_COMPONENT("pos", float3),
      DESCRIBE_QUERY_COMPONENT("forward", float3),
      DESCRIBE_QUERY_COMPONENT("camera_fov", float),
      DESCRIBE_QUERY_COMPONENT("camera_zNear", float),
      DESCRIBE_QUERY_COMPONENT("camera_zFar", float)
    }
  }
);


void query_camera (eastl::function<
  void(
    const float3& pos,
    const float3& forward,
    const float& camera_fov,
    const float& camera_zNear,
    const float& camera_zFar)> cb)
{
  Engine::ECS::manager.query(query_camera_queryId, [&](ComponentsAccessor& accessor)
  {
    const float3& pos = accessor.get<float3>(str_hash("pos"));
    const float3& forward = accessor.get<float3>(str_hash("forward"));
    const float& camera_fov = accessor.get<float>(str_hash("camera_fov"));
    const float& camera_zNear = accessor.get<float>(str_hash("camera_zNear"));
    const float& camera_zFar = accessor.get<float>(str_hash("camera_zFar"));
    cb(pos,forward,camera_fov,camera_zNear,camera_zFar);
  });
}
