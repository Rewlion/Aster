#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/gi_surfels/consts.hlsl>

NODE_BEGIN(gibs_surfels_irradiance)
  ORDER_ME_BEFORE(gibs_sync_out)
  BIND_BUF_RW_UAV_AS(gibs_surfels_storage_binned, surfelsStorage)
  BIND_BUF_SRV_AS   (gibs_surfels_spatial_storage_binned, surfelsSpatialStorage)
  BIND_TEX_RW_UAV_AS(gibs_rayguiding_map, rayguidingMap)
  BIND_TEX_SRV_AS   (atm_sky_lut_srv, skyLUT)
  BIND_BUF_SRV_AS   (gibs_surfels_lifetime, surfelsLifeTime)
  BIND_BUF_SRV_AS   (sph_buf, atmParamsBuffer)


  EXEC(gibs_surfels_irradiance)
NODE_END()

NODE_EXEC()
static
void gibs_surfels_irradiance(gapi::CmdEncoder& encoder)
{
  tfx::activate_scope("AtmosphereScope", encoder);
  // {
  //   tfx::activate_scope("AtmosphereScope", encoder);
  //   tfx::activate_technique("GIBS_AccSunIrradiance", encoder);
  //   encoder.updateResources();
  
  //   const uint3 ds = tfx::calc_group_count("GIBS_AccSunIrradiance", uint3(SURFEL_COUNT_TOTAL, 1, 1));
  //   encoder.dispatch(ds.x, ds.y, ds.z);
  // }

  // encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_UAV_RW, gapi::BufferState::BF_STATE_UAV_RW);

  {
    tfx::activate_technique("GIBS_AccSurfelsIrradiance", encoder);
    encoder.updateResources();
  
    const uint3 ds = tfx::calc_group_count("GIBS_AccSurfelsIrradiance", uint3(SURFEL_COUNT_TOTAL, 1, 1));
    encoder.dispatch(ds.x, ds.y, ds.z);
  }
}
