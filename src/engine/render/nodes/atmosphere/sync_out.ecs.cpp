#include "render_state.h"

#include <engine/render/frame_graph/dsl.h>

ECS_COMP_GETTER(AtmosphereRenderState, atmosphere_render_state);

NODE_BEGIN(atm_sync_out)
  ORDER_ME_AFTER(frame_preparing)

  RENAME_TEX(atm_envi_specular, atm_envi_specular_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_envi_brdf, atm_envi_brdf_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_tr_lut, atm_tr_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_ms_lut, atm_ms_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_sky_lut, atm_sky_lut_srv, TEX_STATE(ShaderRead))
  RENAME_TEX(atm_ap_lut, atm_ap_lut_srv, TEX_STATE(ShaderRead))

  EXEC(atm_sync_out_exec)
NODE_END()

NODE_EXEC()
static
void atm_sync_out_exec()
{
  if (AtmosphereRenderState* st = get_atmosphere_render_state())
    st->markReady();
}