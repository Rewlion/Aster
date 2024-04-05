#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_trace_indirect_light)
  ORDER_ME_BEFORE(gibs_sync_out)
  
  RENAME_BUF(gibs_surfels_storage_binned, gibs_surfels_storage_srv, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_spatial_storage_binned, gibs_surfels_spatial_storage_srv, BUF_STATE(SRV))

  CREATE_TEX_2D(gibs_indirect_light_sample, TEX_SIZE_RELATIVE_DIV(2), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))

  BIND_SHADER_VAR_AS   (gibs_indirect_light_sample, outputTex)
  BIND_SHADER_VAR_AS   (gibs_surfels_storage_srv, surfelsStorage)
  BIND_SHADER_VAR_AS   (gibs_surfels_spatial_storage_srv, surfelsSpatialStorage)

  READ_RENDER_SIZE_AS  (render_size)
  BIND_TEX_SRV_AS      (gbuf1, gbuffer_normal)
  BIND_TEX_RO_DEPTH_AS (late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS      (atm_sky_lut_srv, skyLUT)

  EXEC(gibs_indirect_light)
NODE_END()

NODE_EXEC()
static
void gibs_indirect_light(gapi::CmdEncoder& encoder,
                        const uint2& render_size)
{
  tfx::activate_technique("GIBS_IndirectLight", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_IndirectLight", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, gc.z);
}
