#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_indirect_light_sfilter_first_pass)
  RENAME_TEX(gibs_indirect_light_ta_history, gibs_indirect_light_sfilter_feedback, TEX_STATE(ShaderReadWrite))
  
  READ_RENDER_SIZE_AS (render_size)
  BIND_TEX_SRV_AS     (gibs_indirect_light_ta_target, filterInput)
  BIND_SHADER_VAR_AS  (gibs_indirect_light_sfilter_feedback, filterOutput)

  BIND_TEX_SRV_AS     (gibs_indirect_light_variance, variance)
  BIND_TEX_RO_DEPTH_AS(late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS     (gbuf1, gbuffer_normal)

  EXEC(gibs_indirect_light_sfilter_first_pass)
NODE_END()

NODE_EXEC()
static
void gibs_indirect_light_sfilter_first_pass(gapi::CmdEncoder& encoder,
                                            const uint2& render_size)
{
  tfx::set_extern("stepSize", uint(1));

  tfx::activate_technique("GIBS_IndirectLightSpatialFilter", encoder);
  encoder.updateResources();

  const uint3 gc = tfx::calc_group_count("GIBS_IndirectLightSpatialFilter", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, gc.z);
}


NODE_BEGIN(gibs_indirect_light_sfilter_rest_passes)
  CREATE_TEX_2D(gibs_indirect_light_sfilter_tex_one, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))
  CREATE_TEX_2D(gibs_indirect_light_sfilter_tex_two, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))
  
  READ_RENDER_SIZE_AS (render_size)
  READ_TEX_SRV(gibs_indirect_light_sfilter_feedback)

  BIND_TEX_SRV_AS     (gibs_indirect_light_variance, variance)
  BIND_TEX_RO_DEPTH_AS(late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS     (gbuf1, gbuffer_normal)

  EXEC(gibs_indirect_light_sfilter_rest_passes)
NODE_END()

NODE_EXEC()
static
void gibs_indirect_light_sfilter_rest_passes(gapi::CmdEncoder& encoder,
                                            const uint2& render_size,
                                            const gapi::TextureHandle gibs_indirect_light_sfilter_tex_one,
                                            const gapi::TextureHandle gibs_indirect_light_sfilter_tex_two,
                                            const gapi::TextureHandle gibs_indirect_light_sfilter_feedback)
{
  const auto dispatchStep = [&](uint stepSize) {
    tfx::set_extern("stepSize", stepSize);

    tfx::activate_technique("GIBS_IndirectLightSpatialFilter", encoder);
    encoder.updateResources();

    const uint3 gc = tfx::calc_group_count("GIBS_IndirectLightSpatialFilter", uint3{render_size, 1});
    encoder.dispatch(gc.x, gc.y, gc.z);
  };
  
  gapi::TextureViewWithState texOne(gibs_indirect_light_sfilter_tex_one, gapi::TextureState::ShaderReadWrite);
  gapi::TextureViewWithState texTwo(gibs_indirect_light_sfilter_tex_two, gapi::TextureState::ShaderReadWrite);

  tfx::set_extern("filterInput", gibs_indirect_light_sfilter_feedback);
  tfx::set_extern("filterOutput", texOne);
  dispatchStep(2);

  texOne.transitState(encoder, gapi::TextureState::ShaderRead);
  tfx::set_extern("filterInput", texOne);
  tfx::set_extern("filterOutput", texTwo);
  dispatchStep(4);

  texOne.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  texTwo.transitState(encoder, gapi::TextureState::ShaderRead);
  tfx::set_extern("filterInput", texTwo);
  tfx::set_extern("filterOutput", texOne);
  dispatchStep(8);

  texOne.transitState(encoder, gapi::TextureState::ShaderRead);
  texTwo.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  tfx::set_extern("filterInput", texOne);
  tfx::set_extern("filterOutput", texTwo);
  dispatchStep(16);

  texOne.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  texTwo.transitState(encoder, gapi::TextureState::ShaderRead);
  tfx::set_extern("filterInput", texTwo);
  tfx::set_extern("filterOutput", texOne);
  dispatchStep(32);

  texTwo.transitState(encoder, gapi::TextureState::ShaderReadWrite);
}

NODE_BEGIN(gibs_indirect_light_end)
  RENAME_TEX(gibs_indirect_light_sfilter_tex_one, gibs_indirect_light_srv, TEX_STATE(ShaderRead))
  NO_EXEC()
NODE_END()
