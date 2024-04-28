#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_ms_lut_render)
  BIND_TEX_SRV_AS(atm_tr_lut, trLUT)

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_ms_lut)
  RP_END()

  EXEC(atm_ms_lut_render_exec)
NODE_END()

NODE_EXEC()
static
void atm_ms_lut_render_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_technique("MultipleScatteringLUT", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
