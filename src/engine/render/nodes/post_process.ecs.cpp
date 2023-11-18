#include <engine/render/frame_graph/dsl.h>
#include <engine/tfx/tfx.h>

NODE_BEGIN(post_process_input_producer)
  RENAME_TEX_RT(transparent_target, post_process_input)
  NO_EXEC()
NODE_END()

NODE_BEGIN(post_process)
  BIND_TEX_SRV_AS(post_process_input, post_process_input)
  CREATE_TEX_2D(final_target, TEX_SIZE_RELATIVE(), R8G8B8A8_UNORM, TEX_USAGE3(RT,SRV, TRANSFER_SRC), TEX_STATE(RenderTarget))
  RP_BEGIN()
    TARGET(final_target)
  RP_END()
  EXEC(post_process_exec)
NODE_END()

NODE_EXEC()
static
void post_process_exec(gapi::CmdEncoder& encoder)
{
  tfx::activate_scope("PostProcessScope", encoder);
  tfx::activate_technique("PostProcess", encoder);

  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}
