#include <engine/render/frame_graph/dsl.h>
#include <engine/render/nodes/utils.h>

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

