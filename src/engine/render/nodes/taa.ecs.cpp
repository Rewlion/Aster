#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(TAA)
  BIND_TEX_SRV_AS(final_target, taaCurrentFrame)
  BIND_PREV_FRAME_TEX_SRV_AS(final_antialiased_target, taaPrevFrame)
  BIND_TEX_SRV_AS(motionBuf, motionBuf)
  BIND_TEX_RO_DEPTH_AS(gbuffer_depth, gbuffer_depth)

  CREATE_TEX_2D(final_antialiased_target, TEX_SIZE_RELATIVE(), R8G8B8A8_UNORM, TEX_USAGE3(RT,SRV,TRANSFER_SRC), TEX_STATE(RenderTarget))

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(final_antialiased_target)
  RP_END()

  EXEC(taa_exec)
NODE_END()

NODE_EXEC()
static
void taa_exec(gapi::CmdEncoder& encoder)
{
  tfx::set_extern("renderSize", encoder.getRenderSize());
  tfx::activate_technique("TAA", encoder);

  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
