#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(TAA)
  BIND_TEX_SRV_AS(post_opaque_target, taaCurrentFrame)
  READ_PREV_FRAME_TEX_AS(taa_target, taa_target_prev, TEX_STATE(ShaderRead))
  BIND_SHADER_VAR_AS(taa_target_prev, taaPrevFrame)
  BIND_TEX_SRV_AS(motionBuf, motionBuf)
  BIND_TEX_RO_DEPTH_AS(gbuffer_depth, gbuffer_depth)

  CREATE_TEX_2D(taa_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(taa_target)
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
