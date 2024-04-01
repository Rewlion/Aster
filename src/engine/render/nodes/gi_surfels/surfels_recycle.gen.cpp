// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/ecs.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "surfels_recycle.ecs.cpp" 

using namespace ecs;

//Engine::OnFrameGraphInit handler
static
void mk_fg_node_gibs_surfels_recycle(Event*, ComponentsAccessor&)
{
  fg::register_node("gibs_surfels_recycle", FG_FILE_DECL, [](fg::Registry& reg)
  { 
    reg.orderMeBefore("gibs_spatial_storage_binning");
    auto gibs_surfels_lifetime = reg.modifyBuffer("gibs_surfels_lifetime", gapi::BufferState::BF_STATE_UAV_RW);
    auto gibs_surfels_pool = reg.modifyBuffer("gibs_surfels_pool", gapi::BufferState::BF_STATE_UAV_RW);
    auto gibs_surfels_meta = reg.modifyBuffer("gibs_surfels_meta", gapi::BufferState::BF_STATE_UAV_RW);

    return [gibs_surfels_lifetime,gibs_surfels_pool,gibs_surfels_meta](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("surfelsLifeTime", gibs_surfels_lifetime.get());
      tfx::set_extern("surfelsPool", gibs_surfels_pool.get());
      tfx::set_extern("surfelsMeta", gibs_surfels_meta.get());
      gibs_surfels_recycle(encoder);
    };
  });
}

static
EventSystemRegistration mk_fg_node_gibs_surfels_recycle_registration(
  mk_fg_node_gibs_surfels_recycle,
  compile_ecs_name_hash("OnFrameGraphInit"),
  {
  },
  "mk_fg_node_gibs_surfels_recycle"
);