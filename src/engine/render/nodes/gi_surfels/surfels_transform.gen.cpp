// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "surfels_transform.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_transform_surfels(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_transform_surfels", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("gibs_sync_out");
    auto gibs_surfels_storage_binned = reg.modifyBuffer("gibs_surfels_storage_binned", gapi::BufferState::BF_STATE_UAV_RW);
    auto gibs_surfels_lifetime = reg.readBuffer("gibs_surfels_lifetime", gapi::BufferState::BF_STATE_SRV, false);
    auto gibs_rayguiding_map = reg.modifyTexture("gibs_rayguiding_map", gapi::TextureState::ShaderReadWrite);
    auto gibs_surfels_ray_budget = reg.modifyBuffer("gibs_surfels_ray_budget", gapi::BufferState::BF_STATE_UAV_RW);

    return [gibs_surfels_storage_binned,gibs_surfels_lifetime,gibs_rayguiding_map,gibs_surfels_ray_budget](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("surfelsStorage", gibs_surfels_storage_binned.get());
      tfx::set_extern("surfelsLifeTime", gibs_surfels_lifetime.get());
      tfx::set_extern("rayguidingMap", gibs_rayguiding_map.get());
      tfx::set_extern("surfelsRayBudget", gibs_surfels_ray_budget.get());
      gibs_transform_surfels(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_transform_surfels_registration(
  mk_fg_node_gibs_transform_surfels,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_transform_surfels"
);