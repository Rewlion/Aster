#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_sky_apply)
  BIND_TEX_SRV_AS(atm_ap_lut, apLUT)
  BIND_TEX_SRV_AS(atm_sky_lut, skyLUT)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)
  BIND_TEX_RO_DEPTH_AS(gbuffer_depth, gbufferDepth)

  RP_BEGIN()
    TARGET(resolve_target)
  RP_END()

  EXEC(atm_sky_apply_exec)
NODE_END()

NODE_EXEC()
static
void atm_sky_apply_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("SkyApply", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
