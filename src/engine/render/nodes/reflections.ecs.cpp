#include "utils.h"

#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(reflections_resources)
  CREATE_TEX_2D ( reflections_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )

  CREATE_TEX_2D ( reflections_acc, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )

  CREATE_TEX_2D ( reflections_variance, TEX_SIZE_RELATIVE(), R8_UNORM,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )

  CREATE_TEX_2D ( reflections_target_filtered, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )

  EXEC(reflections_resources)
NODE_END()

NODE_EXEC()
static
void reflections_resources(gapi::CmdEncoder& encoder,
                           const gapi::TextureHandle reflections_target,
                           const gapi::TextureHandle reflections_acc,
                           const gapi::TextureHandle reflections_variance,
                           const gapi::TextureHandle reflections_target_filtered)
{
  encoder.clearColorTexture(reflections_target,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(reflections_acc,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(reflections_variance,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});

  encoder.clearColorTexture(reflections_target_filtered,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});
}

NODE_BEGIN(reflections)
  READ_RENDER_SIZE_AS       (render_size)
  BIND_TEX_RW_UAV_AS        (reflections_target, reflectionsTarget)
  BIND_TEX_SRV_AS           (gbuf0, gbuffer_albedo)
  BIND_TEX_SRV_AS           (gbuf1, gbuffer_normal)
  BIND_TEX_SRV_AS           (gbuf2, gbuffer_metal_roughness)
  BIND_PREV_FRAME_TEX_SRV_AS(post_process_input, prevPostProcessInput)
  BIND_TEX_SRV_AS           (motionBuf, motionBuf)
  BIND_TEX_RO_DEPTH_AS      (late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS           (hi_z_buffer, hi_z_buffer)
  BIND_OPTIONAL_TEX_SRV_AS  (atm_envi_specular_srv, enviSpecular)
  BIND_OPTIONAL_TEX_SRV_AS  (atm_envi_brdf_srv, enviBRDF)

  EXEC(reflections)
NODE_END()

NODE_EXEC()
static
void reflections(gapi::CmdEncoder& encoder,
                 const uint2& render_size)
{
  tfx::activate_scope("SpecularReflectionScope", encoder);
  tfx::activate_technique("SpecularReflection", encoder);
  
  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("SpecularReflection", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, 1);
}

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

NODE_BEGIN(reflections_blur)
  READ_RENDER_SIZE_AS (render_size)
  BIND_TEX_SRV_AS     (reflections_acc, blurInput)
  BIND_TEX_RW_UAV_AS  (reflections_target_filtered, blurOutput)
  BIND_TEX_SRV_AS     (gbuf2, gbuffer_metal_roughness)
  BIND_TEX_SRV_AS     (reflections_variance, taVariance)

  EXEC(reflections_blur)
NODE_END()

NODE_EXEC()
static
void reflections_blur(gapi::CmdEncoder& encoder, const uint2& render_size)
{
  tfx::activate_scope("ReflectionsFilterScope", encoder);
  tfx::activate_technique("ReflectionsFilter", encoder);

  encoder.updateResources();
  const uint3 gc = tfx::calc_group_count("ReflectionsFilter", uint3{render_size, 1});
  encoder.dispatch(gc.x, gc.y, 1);
}

NODE_BEGIN(reflections_sync)
  RENAME_TEX(reflections_target_filtered, specular_light, TEX_STATE(ShaderRead))
  NO_EXEC()
NODE_END()
