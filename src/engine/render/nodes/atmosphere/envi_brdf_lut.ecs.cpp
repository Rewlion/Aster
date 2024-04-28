#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(atm_envi_brdf)
  RP_BEGIN()
    TARGET_LOAD_DONTCARE(atm_envi_brdf)
  RP_END()
  EXEC(atm_envi_brdf_exec)
NODE_END()

NODE_EXEC()
static
void atm_envi_brdf_exec(gapi::CmdEncoder& encoder)
{
  tfx::set_extern("renderSize", encoder.getRenderSize());
  tfx::activate_technique("EnviBRDF", encoder);
  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}