#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>

NODE_BEGIN(gibs_surfels_recycle)
  ORDER_ME_BEFORE(gibs_spatial_storage_binning)

  BIND_BUF_RW_UAV_AS (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_RW_UAV_AS (gibs_surfels_pool, surfelsPool)
  BIND_BUF_RW_UAV_AS (gibs_surfels_meta, surfelsMeta)

  EXEC(gibs_surfels_recycle)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_recycle(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("GIBS_SurfelsRecycle", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_SurfelsRecycle", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}
