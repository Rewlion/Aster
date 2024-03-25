#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_indirect_light_ta)

  CREATE_TEX_2D(gibs_indirect_light_variance, TEX_SIZE_RELATIVE(), R8_UNORM, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))
  CREATE_TEX_2D(gibs_indirect_light_ta_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))
  CREATE_TEX_2D(gibs_indirect_light_ta_history, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))

  READ_PREV_FRAME_TEX_AS(gibs_indirect_light_ta_history, gibs_indirect_light_ta_history_prev, TEX_STATE(ShaderRead))
  READ_PREV_FRAME_TEX_AS(gibs_indirect_light_variance, gibs_indirect_light_variance_prev, TEX_STATE(ShaderRead))
  
  READ_RENDER_SIZE_AS (render_size)
  BIND_SHADER_VAR_AS  (gibs_indirect_light_variance, varianceOutput)
  BIND_SHADER_VAR_AS  (gibs_indirect_light_variance_prev, varianceHistory)
  BIND_SHADER_VAR_AS  (gibs_indirect_light_ta_target, taOutput)
  BIND_SHADER_VAR_AS  (gibs_indirect_light_ta_history_prev, taHistory)
  BIND_TEX_SRV_AS     (gibs_indirect_light_sample, inputIrradiance)

  BIND_TEX_SRV_AS     (motionBuf, motionBuf)
  BIND_TEX_RO_DEPTH_AS(late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS     (gbuf1, gbuffer_normal)

  READ_PREV_FRAME_RO_DEPTH_AS(late_opaque_depth, late_opaque_depth_prev)
  BIND_SHADER_VAR_AS         (late_opaque_depth_prev, gbuffer_depth_prev)
  READ_PREV_FRAME_TEX_SRV_AS (gbuf1, gbuf1_prev)
  BIND_SHADER_VAR_AS         (gbuf1_prev, gbuffer_normal_prev)

  EXEC(gibs_indirect_light_temporal_acc)
NODE_END()

NODE_EXEC()
static
void gibs_indirect_light_temporal_acc(gapi::CmdEncoder& encoder, const uint2 render_size)
{
  tfx::activate_technique("GIBS_IndirectLightTemporalAccumulation", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_IndirectLightTemporalAccumulation", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, gc.z);
}
