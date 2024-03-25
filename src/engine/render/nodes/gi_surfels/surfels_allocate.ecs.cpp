#include "dbg_flags.h"

#include <engine/render/frame_graph/dsl.h>
#include <engine/time.h>

constexpr uint64_t SLOW_ALLOC_PERIOD_MS = 150;
static uint next_slow_alloc_time_ms = 0;

NODE_BEGIN(gibs_allocate_surfels)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RW_UAV_AS   (gibs_dbg_alloc, dbgTex)
  BIND_TEX_SRV_AS      (gbuf1, gbuffer_normal)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_pool_binned, surfelsPool)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_allocation_locks, surfelsAllocLocks)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_meta_binned, surfelsMeta)
  BIND_TEX_SRV_AS      (gibs_surfels_allocation_pos, surfelsAllocPos)
  BIND_TEX_RW_UAV_AS   (gibs_rayguiding_map, rayguidingMap)

  ORDER_ME_BEFORE(gbuffer_resolve)

  EXEC(gibs_allocate_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_allocate_surfels(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  if (gibs_freeze_allocation)
    return;

  if (gibs_slow_allocation)
  {
    uint64_t curMs = Engine::Time::get_time_ms_since_start();
    if (curMs >= next_slow_alloc_time_ms)
      next_slow_alloc_time_ms = curMs + SLOW_ALLOC_PERIOD_MS;
    else
      return;
  }

  tfx::activate_technique("GIBS_AllocateSurfels", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_AllocateSurfels", uint3(render_size, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}
