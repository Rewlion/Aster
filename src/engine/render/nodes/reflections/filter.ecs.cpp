
#include <engine/render/frame_graph/dsl.h>

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
