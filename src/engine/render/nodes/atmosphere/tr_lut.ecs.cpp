#include "render_state.h"

#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_tr_lut_render)
  ORDER_ME_AFTER(frame_preparing)

  RP_BEGIN()
    TARGET(atm_tr_lut)
  RP_END()

  EXEC(atm_tr_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_tr_lut_render_exec(gapi::CmdEncoder& encoder)
{
  if (!AtmosphereRenderState::isDirty())
    return;

  tfx::activate_technique("TransmittanceLUT", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
