#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_surfels_find_surfels_alloc_pos)
  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_sdf, surfelsSDF)
  BIND_BUF_SRV_AS      (gibs_surfels_storage, surfelsStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_spatial_storage, surfelsSpatialStorage)
  BIND_TEX_RW_UAV_AS   (gibs_surfels_allocation_pos, surfelsAllocPos)
  BIND_BUF_RW_UAV_AS   (gibs_surfels_lifetime, surfelsLifeTime)

  EXEC(gibs_surfels_find_surfels_alloc_pos)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_find_surfels_alloc_pos(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  tfx::activate_technique("GIBS_FindSurfelsAllocPos", encoder);
  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("GIBS_FindSurfelsAllocPos", uint3(render_size, 1));
  encoder.dispatch(gc.x, gc.y, gc.z);
}
