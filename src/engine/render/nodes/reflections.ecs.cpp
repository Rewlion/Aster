#include "utils.h"

#include <engine/render/frame_graph/dsl.h>

#include <engine/shaders/shaders/reflections.hlsl>

NODE_BEGIN(reflections_resources)
  CREATE_TEX_2D ( reflections_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )
  EXEC(reflections_resources)
NODE_END()

NODE_EXEC()
static
void reflections_resources(gapi::CmdEncoder& encoder, gapi::TextureHandle reflections_target)
{
  encoder.clearColorTexture(reflections_target,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});
}

NODE_BEGIN(reflections)
  READ_RENDER_SIZE_AS       (render_size)
  BIND_TEX_RW_UAV_AS        (reflections_target, reflectionsTarget)
  BIND_TEX_SRV_AS           (gbuf1, gbuffer_normal)
  BIND_TEX_SRV_AS           (gbuf2, gbuffer_metal_roughness)
  BIND_PREV_FRAME_TEX_SRV_AS(post_process_input, prevPostProcessInput)
  BIND_TEX_SRV_AS           (motionBuf, motionBuf)
  BIND_TEX_RO_DEPTH_AS      (late_opaque_depth, gbuffer_depth)
  BIND_TEX_SRV_AS           (hi_z_buffer, hi_z_buffer)
  BIND_OPTIONAL_TEX_SRV_AS  (atm_envi_specular, enviSpecular)
  BIND_OPTIONAL_TEX_SRV_AS  (atm_envi_brdf, enviBRDF)
  EXEC(reflections)
NODE_END()

NODE_EXEC()
static
void reflections(gapi::CmdEncoder& encoder,
                 const uint2 render_size)
{
  tfx::activate_scope("SpecularReflectionScope", encoder);
  tfx::activate_technique("SpecularReflection", encoder);
  
  encoder.updateResources();
  encoder.dispatch(get_group_size(render_size.x, TILE_DIM_X),
                   get_group_size(render_size.y, TILE_DIM_X),
                   1);
}

NODE_BEGIN(reflections_sync)
  RENAME_TEX(reflections_target, specular_light, TEX_STATE(ShaderRead))
  NO_EXEC()
NODE_END()
