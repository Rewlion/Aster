#include"dbg_flags.h"

#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_draw_surfels)
  ORDER_ME_BEFORE(gibs_sync_out)

  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_BUF_SRV_AS      (gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_spatial_storage_binned, surfelsSpatialStorage)
  BIND_BUF_SRV_AS      (gibs_surfels_meta_binned, surfelsMeta)
  BIND_BUF_SRV_AS      (gibs_surfels_ray_budget, surfelsRayBudget)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(gibs_dbg_surfels)
  RP_END()

  EXEC(gibs_draw_surfels)
NODE_END()

NODE_EXEC()
static
void gibs_draw_surfels(gapi::CmdEncoder& encoder)
{
  tfx::set_extern("showSurfelsCoverage", gibs_show_surfels_coverage ? (uint)1 : (uint)0);
  tfx::set_extern("showSurfelSDF", gibs_show_surfels_sdf ? (uint)1 : (uint)0);
  tfx::set_extern("showCellsCoverage", gibs_show_cells_coverage ? (uint)1 : (uint)0);
  tfx::set_extern("showCells", gibs_show_cells ? (uint)1 : (uint)0);
  tfx::set_extern("showSurfelsIrradiance", gibs_show_surfels_irradiance ? (uint)1:(uint)0);
  tfx::activate_technique("GIBS_DrawSurfels", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
