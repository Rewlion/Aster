
#include <engine/render/frame_graph/dsl.h>
#include <engine/debug_marks.h>

NODE_BEGIN(reflections_spatial_filter)
  RENAME_TEX(reflections_ta_history, reflections_sfilter_feedback, TEX_STATE(ShaderReadWrite))

  CREATE_TEX_2D(reflections_sfilter_tex_one, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))
  CREATE_TEX_2D(reflections_sfilter_tex_two, TEX_SIZE_RELATIVE(), R32G32B32A32_S, TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite))

  READ_RENDER_SIZE_AS (render_size)
  BIND_TEX_SRV_AS     (reflections_ta_target, filterInput)
  BIND_SHADER_VAR_AS  (reflections_sfilter_feedback, filterOutput)

  BIND_TEX_SRV_AS     (reflections_variance, variance)
  BIND_TEX_RO_DEPTH_AS(late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS     (gbuf1, gbuffer_normal)
  BIND_TEX_SRV_AS     (gbuf2, gbuffer_metal_roughness)

  EXEC(reflections_spatial_filter)
NODE_END()

NODE_EXEC()
static
void reflections_spatial_filter(gapi::CmdEncoder& encoder,
                                const uint2& render_size,
                                const gapi::TextureHandle reflections_ta_target,
                                const gapi::TextureHandle reflections_sfilter_feedback,
                                const gapi::TextureHandle reflections_sfilter_tex_one,
                                const gapi::TextureHandle reflections_sfilter_tex_two
                                )
{
  const uint3 dispatchDim = tfx::calc_group_count("Reflections_SpatialFilter", uint3{render_size, 1});
  tfx::set_extern("dispatchDim", uint2(dispatchDim));

  const auto dispatchStep = [&](int stepSize) {
    tfx::set_extern("stepSize", stepSize);

    tfx::activate_technique("Reflections_SpatialFilter", encoder);
    encoder.updateResources();

    encoder.dispatch(dispatchDim.x, dispatchDim.y, dispatchDim.z);
  };
  
  gapi::TextureViewWithState input(reflections_ta_target, gapi::TextureState::ShaderRead);
  gapi::TextureViewWithState feedback(reflections_sfilter_feedback, gapi::TextureState::ShaderReadWrite);
  gapi::TextureViewWithState texOne(reflections_sfilter_tex_one, gapi::TextureState::ShaderReadWrite);
  gapi::TextureViewWithState texTwo(reflections_sfilter_tex_two, gapi::TextureState::ShaderReadWrite);

  {
    GAPI_MARK("gibs_sfilter_1", encoder);

    tfx::set_extern("filterInput", input);
    tfx::set_extern("filterOutput", feedback);
    dispatchStep(1);
  }

  {
    GAPI_MARK("gibs_sfilter_2", encoder);

    feedback.transitState(encoder, gapi::TextureState::ShaderRead);
    tfx::set_extern("filterInput", feedback);
    tfx::set_extern("filterOutput", texOne);
    dispatchStep(2);
  }

  {
    GAPI_MARK("gibs_sfilter_4", encoder);

    texOne.transitState(encoder, gapi::TextureState::ShaderRead);
    tfx::set_extern("filterInput", texOne);
    tfx::set_extern("filterOutput", texTwo);
    dispatchStep(4);
  }

  {
    GAPI_MARK("gibs_sfilter_8", encoder);

    texOne.transitState(encoder, gapi::TextureState::ShaderReadWrite);
    texTwo.transitState(encoder, gapi::TextureState::ShaderRead);
    tfx::set_extern("filterInput", texTwo);
    tfx::set_extern("filterOutput", texOne);
    dispatchStep(8);
  }

  // {
  //   GAPI_MARK("gibs_sfilter_16", encoder);

  //   texOne.transitState(encoder, gapi::TextureState::ShaderRead);
  //   texTwo.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  //   tfx::set_extern("filterInput", texOne);
  //   tfx::set_extern("filterOutput", texTwo);
  //   dispatchStep(16);
  // }
  

  feedback.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  texTwo.transitState(encoder, gapi::TextureState::ShaderReadWrite);
  // texOne.transitState(encoder, gapi::TextureState::ShaderReadWrite);
}

NODE_BEGIN(reflections_sync)
  // RENAME_TEX(reflections_sfilter_tex_two, specular_light, TEX_STATE(ShaderRead))
  RENAME_TEX(reflections_sfilter_tex_one, specular_light, TEX_STATE(ShaderRead))
  NO_EXEC()
NODE_END()
