#include "dbg_flags.h"

#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>

NODE_BEGIN(gibs_transform_surfels)
  ORDER_ME_BEFORE(gibs_sync_out)
  BIND_BUF_RW_UAV_AS (gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_SRV_AS    (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_TEX_RW_UAV_AS (gibs_rayguiding_map, rayguidingMap)
  BIND_BUF_RW_UAV_AS (gibs_surfels_ray_budget, surfelsRayBudget)

  EXEC(gibs_transform_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_transform_surfels(gapi::CmdEncoder& encoder)
{
  if (!gibs_enable_surfels_transform)
    return;

  tfx::activate_technique("GIBS_TransformSurfels", encoder);
  encoder.updateResources();
  
  const uint3 ds = tfx::calc_group_count("GIBS_TransformSurfels", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(ds.x, ds.y, ds.z);
}
