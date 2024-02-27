// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "test_misc.ecs.cpp" 

using namespace ecs;

static void system_init_internal(Event* event, ComponentsAccessor& accessor)
{
  ecs::OnEntityCreated* casted_event = reinterpret_cast<ecs::OnEntityCreated*>(event);
  float2 test_float2 = accessor.get<float2>(compile_ecs_name_hash("test_float2"));
  system_init(*casted_event, test_float2);
}


static EventSystemRegistration system_init_registration(
  system_init_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("test_float2", float2)
  },
  "system_init"
);


static void system_test_multiple_templates_internal(ComponentsAccessor& accessor)
{
  const float& test_float = accessor.get<float>(compile_ecs_name_hash("test_float"));
  const string& test_str = accessor.get<string>(compile_ecs_name_hash("test_str"));
  float2 test_float2 = accessor.get<float2>(compile_ecs_name_hash("test_float2"));
  float3 test_float3 = accessor.get<float3>(compile_ecs_name_hash("test_float3"));
  system_test_multiple_templates(test_float,test_str,test_float2,test_float3);
}


static SystemRegistration system_test_multiple_templates_registration(
  system_test_multiple_templates_internal,
  {
    DESCRIBE_QUERY_COMPONENT("test_float", float),
    DESCRIBE_QUERY_COMPONENT("test_str", string),
    DESCRIBE_QUERY_COMPONENT("test_float2", float2),
    DESCRIBE_QUERY_COMPONENT("test_float3", float3)
  },
  "system_test_multiple_templates"
);


static void system_test_default_str_internal(ComponentsAccessor& accessor)
{
  const string& default_str = accessor.get<string>(compile_ecs_name_hash("default_str"));
  system_test_default_str(default_str);
}


static SystemRegistration system_test_default_str_registration(
  system_test_default_str_internal,
  {
    DESCRIBE_QUERY_COMPONENT("default_str", string)
  },
  "system_test_default_str"
);


static void system_test_test_float2_internal(ComponentsAccessor& accessor)
{
  float2 test_float2 = accessor.get<float2>(compile_ecs_name_hash("test_float2"));
  system_test_test_float2(test_float2);
}


static SystemRegistration system_test_test_float2_registration(
  system_test_test_float2_internal,
  {
    DESCRIBE_QUERY_COMPONENT("test_float2", float2)
  },
  "system_test_test_float2"
);


static void system_src_tmpl_internal(ComponentsAccessor& accessor)
{
  float fl_data = accessor.get<float>(compile_ecs_name_hash("fl_data"));
  const string& txt_data = accessor.get<string>(compile_ecs_name_hash("txt_data"));
  system_src_tmpl(fl_data,txt_data);
}


static SystemRegistration system_src_tmpl_registration(
  system_src_tmpl_internal,
  {
    DESCRIBE_QUERY_COMPONENT("fl_data", float),
    DESCRIBE_QUERY_COMPONENT("txt_data", string)
  },
  "system_src_tmpl"
);


static void system_dst_tmpl_internal(ComponentsAccessor& accessor)
{
  float fl_data = accessor.get<float>(compile_ecs_name_hash("fl_data"));
  const string& txt_data2 = accessor.get<string>(compile_ecs_name_hash("txt_data2"));
  system_dst_tmpl(fl_data,txt_data2);
}


static SystemRegistration system_dst_tmpl_registration(
  system_dst_tmpl_internal,
  {
    DESCRIBE_QUERY_COMPONENT("fl_data", float),
    DESCRIBE_QUERY_COMPONENT("txt_data2", string)
  },
  "system_dst_tmpl"
);


static void system_flag_internal(ComponentsAccessor& accessor)
{
  bool flag = accessor.get<bool>(compile_ecs_name_hash("flag"));
  system_flag(flag);
}


static SystemRegistration system_flag_registration(
  system_flag_internal,
  {
    DESCRIBE_QUERY_COMPONENT("flag", bool)
  },
  "system_flag"
);


static void system_on_pawn_creation_internal(Event* event, ComponentsAccessor& accessor)
{
  const ecs::OnEntityCreated* casted_event = reinterpret_cast<const ecs::OnEntityCreated*>(event);
  float pawn_pos = accessor.get<float>(compile_ecs_name_hash("pawn_pos"));
  system_on_pawn_creation(*casted_event, pawn_pos);
}


static EventSystemRegistration system_on_pawn_creation_registration(
  system_on_pawn_creation_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("pawn_pos", float)
  },
  "system_on_pawn_creation"
);


const static DirectQueryRegistration query_pawn_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("pawn_pos", float)
  },
  "query_pawn"};
const static query_id_t query_pawn_queryId = query_pawn_queryReg.getId();


void query_pawn (EntityId eid, eastl::function<
  void(
    float pawn_pos)> cb)
{
  ecs::get_registry().query(eid, [&](ComponentsAccessor& accessor)
  {
    float pawn_pos = accessor.get<float>(compile_ecs_name_hash("pawn_pos"));
    cb(pawn_pos);
  });
}


static void system_controller_internal(ComponentsAccessor& accessor)
{
  ecs::EntityId pawn_eid = accessor.get<ecs::EntityId>(compile_ecs_name_hash("pawn_eid"));
  bool controller_flag = accessor.get<bool>(compile_ecs_name_hash("controller_flag"));
  system_controller(pawn_eid,controller_flag);
}


static SystemRegistration system_controller_registration(
  system_controller_internal,
  {
    DESCRIBE_QUERY_COMPONENT("pawn_eid", ecs::EntityId),
    DESCRIBE_QUERY_COMPONENT("controller_flag", bool)
  },
  "system_controller"
);


static void system_on_moving_decal_creation_internal(Event* event, ComponentsAccessor& accessor)
{
  const ecs::OnEntityCreated* casted_event = reinterpret_cast<const ecs::OnEntityCreated*>(event);
  float3& center_pos = accessor.get<float3>(compile_ecs_name_hash("center_pos"));
  const float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
  system_on_moving_decal_creation(*casted_event, center_pos,pos);
}


static EventSystemRegistration system_on_moving_decal_creation_registration(
  system_on_moving_decal_creation_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("center_pos", float3),
    DESCRIBE_QUERY_COMPONENT("pos", float3)
  },
  "system_on_moving_decal_creation"
);


static void moving_decal_controller_internal(ComponentsAccessor& accessor)
{
  bool moving_decal_flag = accessor.get<bool>(compile_ecs_name_hash("moving_decal_flag"));
  const float3& center_pos = accessor.get<float3>(compile_ecs_name_hash("center_pos"));
  float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
  moving_decal_controller(moving_decal_flag,center_pos,pos);
}


static SystemRegistration moving_decal_controller_registration(
  moving_decal_controller_internal,
  {
    DESCRIBE_QUERY_COMPONENT("moving_decal_flag", bool),
    DESCRIBE_QUERY_COMPONENT("center_pos", float3),
    DESCRIBE_QUERY_COMPONENT("pos", float3)
  },
  "moving_decal_controller"
);


const static DirectQueryRegistration query_camera_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("pos", float3),
    DESCRIBE_QUERY_COMPONENT("camera_rotations", float2),
    DESCRIBE_QUERY_COMPONENT("forward", float3)
  },
  "query_camera"};
const static query_id_t query_camera_queryId = query_camera_queryReg.getId();


void query_camera (eastl::function<
  void(
    const float3& pos,
    const float2& camera_rotations,
    const float3& forward)> cb)
{
  ecs::get_registry().query(query_camera_queryId, [&](ComponentsAccessor& accessor)
  {
    const float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
    const float2& camera_rotations = accessor.get<float2>(compile_ecs_name_hash("camera_rotations"));
    const float3& forward = accessor.get<float3>(compile_ecs_name_hash("forward"));
    cb(pos,camera_rotations,forward);
  });
}


static void tick_collision_tests_internal(Event* event, ComponentsAccessor& accessor)
{
  Engine::OnGameTick* casted_event = reinterpret_cast<Engine::OnGameTick*>(event);
  bool collision_test_tag = accessor.get<bool>(compile_ecs_name_hash("collision_test_tag"));
  const float3& center = accessor.get<float3>(compile_ecs_name_hash("center"));
  tick_collision_tests(*casted_event, collision_test_tag,center);
}


static EventSystemRegistration tick_collision_tests_registration(
  tick_collision_tests_internal,
  compile_ecs_name_hash("OnGameTick"),
  {
    DESCRIBE_QUERY_COMPONENT("collision_test_tag", bool),
    DESCRIBE_QUERY_COMPONENT("center", float3)
  },
  "tick_collision_tests"
);


static void move_point_light_internal(ComponentsAccessor& accessor)
{
  const float3& center_pos = accessor.get<float3>(compile_ecs_name_hash("center_pos"));
  float3& pos = accessor.get<float3>(compile_ecs_name_hash("pos"));
  bool moving_point_light_tag = accessor.get<bool>(compile_ecs_name_hash("moving_point_light_tag"));
  move_point_light(center_pos,pos,moving_point_light_tag);
}


static SystemRegistration move_point_light_registration(
  move_point_light_internal,
  {
    DESCRIBE_QUERY_COMPONENT("center_pos", float3),
    DESCRIBE_QUERY_COMPONENT("pos", float3),
    DESCRIBE_QUERY_COMPONENT("moving_point_light_tag", bool)
  },
  "move_point_light"
);


static void render_bvh_test_internal(ComponentsAccessor& accessor)
{
  const string& test_static_mesh_model = accessor.get<string>(compile_ecs_name_hash("test_static_mesh_model"));
  const float3& test_static_mesh_pos = accessor.get<float3>(compile_ecs_name_hash("test_static_mesh_pos"));
  const float3& test_static_mesh_rot = accessor.get<float3>(compile_ecs_name_hash("test_static_mesh_rot"));
  const float3& test_static_mesh_scale = accessor.get<float3>(compile_ecs_name_hash("test_static_mesh_scale"));
  bool bvh_test_flag = accessor.get<bool>(compile_ecs_name_hash("bvh_test_flag"));
  render_bvh_test(test_static_mesh_model,test_static_mesh_pos,test_static_mesh_rot,test_static_mesh_scale,bvh_test_flag);
}


static SystemRegistration render_bvh_test_registration(
  render_bvh_test_internal,
  {
    DESCRIBE_QUERY_COMPONENT("test_static_mesh_model", string),
    DESCRIBE_QUERY_COMPONENT("test_static_mesh_pos", float3),
    DESCRIBE_QUERY_COMPONENT("test_static_mesh_rot", float3),
    DESCRIBE_QUERY_COMPONENT("test_static_mesh_scale", float3),
    DESCRIBE_QUERY_COMPONENT("bvh_test_flag", bool)
  },
  "render_bvh_test"
);


static void render_tlas_test_internal(ComponentsAccessor& accessor)
{
  const float3& tlas_test_center = accessor.get<float3>(compile_ecs_name_hash("tlas_test_center"));
  const float3& tlas_test_forward = accessor.get<float3>(compile_ecs_name_hash("tlas_test_forward"));
  render_tlas_test(tlas_test_center,tlas_test_forward);
}


static SystemRegistration render_tlas_test_registration(
  render_tlas_test_internal,
  {
    DESCRIBE_QUERY_COMPONENT("tlas_test_center", float3),
    DESCRIBE_QUERY_COMPONENT("tlas_test_forward", float3)
  },
  "render_tlas_test"
);
