#include "render_state.h"

#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_sph_render)
  BIND_BUF_RW_UAV_AS(sph_buf, atmParamsBuffer)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)

  EXEC(atm_sph_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_sph_render_exec(gapi::CmdEncoder& encoder)
{
  //todo fix sph_buf creation place
  // if (!AtmosphereRenderState::isDirty())
  //   return;

  tfx::activate_technique("AtmSphericalHarmonics", encoder);
  encoder.updateResources();
  encoder.dispatch(1,1,1);
}
