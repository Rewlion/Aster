#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(reflections_temporal_acc)
  READ_RENDER_SIZE_AS       (render_size)
  BIND_TEX_SRV_AS           (motionBuf, motionBuf)
  BIND_TEX_SRV_AS           (reflections_target, taInput)
  BIND_PREV_FRAME_TEX_SRV_AS(reflections_target_filtered, taHistory)

  READ_PREV_FRAME_TEX_AS(reflections_variance, reflections_variance_prev, TEX_STATE(ShaderRead))
  BIND_SHADER_VAR_AS(reflections_variance_prev, taVarianceHistory)
  MODIFY_TEX_UAV_AS(reflections_variance, reflections_variance_cur)
  BIND_SHADER_VAR_AS(reflections_variance_cur, taVarianceOutput)

  BIND_TEX_RW_UAV_AS        (reflections_acc, taOutput)
  BIND_TEX_RO_DEPTH_AS      (late_opaque_depth, gbuffer_depth)
  EXEC(reflections_temporal_acc)
NODE_END()

NODE_EXEC()
static
void reflections_temporal_acc(gapi::CmdEncoder& encoder, const uint2& render_size)
{
  tfx::activate_scope("ReflectionsTemporalAccumulationScope", encoder);
  tfx::activate_technique("ReflectionsTemporalAccumulation", encoder);
  
  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("ReflectionsTemporalAccumulation", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, 1);
}