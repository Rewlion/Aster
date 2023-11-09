// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "clustered_lights.ecs.cpp" 

using namespace ecs;

const static DirectQueryRegistration query_clustered_lights_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("clustered_lights", Engine::Render::ClusteredLights)
  },
  "query_clustered_lights"};
const static query_id_t query_clustered_lights_queryId = query_clustered_lights_queryReg.getId();


void query_clustered_lights (eastl::function<
  void(
    Engine::Render::ClusteredLights& clustered_lights)> cb)
{
  ecs::get_registry().query(query_clustered_lights_queryId, [&](ComponentsAccessor& accessor)
  {
    Engine::Render::ClusteredLights& clustered_lights = accessor.get<Engine::Render::ClusteredLights>(compile_ecs_name_hash("clustered_lights"));
    cb(clustered_lights);
  });
}


static void point_light_dbg_draw_internal(Event* event, ComponentsAccessor& accessor)
{
  Engine::OnGameTick* casted_event = reinterpret_cast<Engine::OnGameTick*>(event);
  const float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
  const float3& point_light_color = accessor.get<float3>(compile_ecs_name_hash("point_light_color"));
  float point_light_attenuation_radius = accessor.get<float>(compile_ecs_name_hash("point_light_attenuation_radius"));
  point_light_dbg_draw(*casted_event, pos,point_light_color,point_light_attenuation_radius);
}


static EventSystemRegistration point_light_dbg_draw_registration(
  point_light_dbg_draw_internal,
  compile_ecs_name_hash("OnGameTick"),
  {
    DESCRIBE_QUERY_COMPONENT("pos", float3),
    DESCRIBE_QUERY_COMPONENT("point_light_color", float3),
    DESCRIBE_QUERY_COMPONENT("point_light_attenuation_radius", float)
  },
  "point_light_dbg_draw"
);


static void listen_point_lights_creation_internal(Event* event, ComponentsAccessor& accessor)
{
  ecs::OnEntityCreated* casted_event = reinterpret_cast<ecs::OnEntityCreated*>(event);
  const float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
  const float3& point_light_color = accessor.get<float3>(compile_ecs_name_hash("point_light_color"));
  float point_light_attenuation_radius = accessor.get<float>(compile_ecs_name_hash("point_light_attenuation_radius"));
  listen_point_lights_creation(*casted_event, pos,point_light_color,point_light_attenuation_radius);
}


static EventSystemRegistration listen_point_lights_creation_registration(
  listen_point_lights_creation_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("pos", float3),
    DESCRIBE_QUERY_COMPONENT("point_light_color", float3),
    DESCRIBE_QUERY_COMPONENT("point_light_attenuation_radius", float)
  },
  "listen_point_lights_creation"
);