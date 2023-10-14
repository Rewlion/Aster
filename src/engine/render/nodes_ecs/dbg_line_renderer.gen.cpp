// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "dbg_line_renderer.ecs.cpp" 

using namespace ecs;

static void dbg_line_renderer_tick_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnGameTick* casted_event = reinterpret_cast<const Engine::OnGameTick*>(event);
  Engine::dbg::LineRenderer& dbg_line_renderer = accessor.get<Engine::dbg::LineRenderer>(compile_ecs_name_hash("dbg_line_renderer"));
  dbg_line_renderer_tick(*casted_event, dbg_line_renderer);
}


static EventSystemRegistration dbg_line_renderer_tick_registration(
  dbg_line_renderer_tick_internal,
  compile_ecs_name_hash("OnGameTick"),
  {
    DESCRIBE_QUERY_COMPONENT("dbg_line_renderer", Engine::dbg::LineRenderer)
  },
  "dbg_line_renderer_tick"
);


static void dbg_line_renderer_prepare_internal(Event* event, ComponentsAccessor& accessor)
{
  const Engine::OnBeforeRender* casted_event = reinterpret_cast<const Engine::OnBeforeRender*>(event);
  Engine::dbg::LineRenderer& dbg_line_renderer = accessor.get<Engine::dbg::LineRenderer>(compile_ecs_name_hash("dbg_line_renderer"));
  dbg_line_renderer_prepare(*casted_event, dbg_line_renderer);
}


static EventSystemRegistration dbg_line_renderer_prepare_registration(
  dbg_line_renderer_prepare_internal,
  compile_ecs_name_hash("OnBeforeRender"),
  {
    DESCRIBE_QUERY_COMPONENT("dbg_line_renderer", Engine::dbg::LineRenderer)
  },
  "dbg_line_renderer_prepare"
);


const static DirectQueryRegistration query_dbg_line_renderer_queryReg{
  {
    DESCRIBE_QUERY_COMPONENT("dbg_line_renderer", Engine::dbg::LineRenderer)
  },
  "query_dbg_line_renderer"};
const static query_id_t query_dbg_line_renderer_queryId = query_dbg_line_renderer_queryReg.getId();


void query_dbg_line_renderer (eastl::function<
  void(
    Engine::dbg::LineRenderer& dbg_line_renderer)> cb)
{
  ecs::get_registry().query(query_dbg_line_renderer_queryId, [&](ComponentsAccessor& accessor)
  {
    Engine::dbg::LineRenderer& dbg_line_renderer = accessor.get<Engine::dbg::LineRenderer>(compile_ecs_name_hash("dbg_line_renderer"));
    cb(dbg_line_renderer);
  });
}


static void virtual_terrain_creation_handler_internal(Event* event, ComponentsAccessor& accessor)
{
  const ecs::OnEntityCreated* casted_event = reinterpret_cast<const ecs::OnEntityCreated*>(event);
  Engine::dbg::LineRenderer& dbg_line_renderer = accessor.get<Engine::dbg::LineRenderer>(compile_ecs_name_hash("dbg_line_renderer"));
  virtual_terrain_creation_handler(*casted_event, dbg_line_renderer);
}


static EventSystemRegistration virtual_terrain_creation_handler_registration(
  virtual_terrain_creation_handler_internal,
  compile_ecs_name_hash("OnEntityCreated"),
  {
    DESCRIBE_QUERY_COMPONENT("dbg_line_renderer", Engine::dbg::LineRenderer)
  },
  "virtual_terrain_creation_handler"
);
