#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>

NODE_BEGIN(gibs_spatial_storage_binning)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_spatial_storage, surfelsSpatialStorage)
  BIND_BUF_SRV_AS      (gibs_nonlinear_aabbs, gibsNonlinearAABBs)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_pool, surfelsPool)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_meta, surfelsMeta)

  EXEC(gibs_spatial_storage_binning)  
NODE_END()
 
NODE_EXEC()
static
void gibs_spatial_storage_binning(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("GIBS_SpatialStorageBinning", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_SpatialStorageBinning", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}
