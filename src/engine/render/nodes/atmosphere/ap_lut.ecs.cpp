#include "render_state.h"

#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_ap_lut_render)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_SRV_AS(atm_ms_lut, msLUT)
  BIND_TEX_RW_UAV_AS(atm_ap_lut, apLUT)

  EXEC(atm_ap_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_ap_lut_render_exec(gapi::CmdEncoder& encoder)
{
  if (!AtmosphereRenderState::isDirty())
    return;

  tfx::activate_technique("AerialPerspective", encoder);
  encoder.updateResources();
  encoder.dispatch(1,1,32);
}
