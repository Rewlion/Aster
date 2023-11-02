#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

NODE_BEGIN(post_process)
  READ_TEX(transparent_target, TEX_STATE(ShaderRead))
  CREATE_TEX_2D(final_target, TEX_SIZE_RELATIVE(), R8G8B8A8_UNORM, TEX_USAGE2(RT,SRV), TEX_STATE(RenderTarget))
  RP_BEGIN()
    TARGET(final_target)
  RP_END()
  EXEC(post_process_exec)
NODE_END()

NODE_EXEC()
static
void post_process_exec(gapi::CmdEncoder& encoder, const gapi::TextureHandle transparent_target)
{
  tfx::set_extern("post_process_input", transparent_target);
  tfx::activate_scope("PostProcessScope", encoder);
  tfx::activate_technique("PostProcess", encoder);

  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
