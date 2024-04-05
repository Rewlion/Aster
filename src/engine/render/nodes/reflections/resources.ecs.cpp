#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(reflections_resources)
  CREATE_TEX_2D ( reflections_target, TEX_SIZE_RELATIVE(), R32G32B32A32_S,
                  TEX_USAGE2(UAV,SRV), TEX_STATE(ShaderReadWrite) )

  EXEC(reflections_resources)
NODE_END()

NODE_EXEC()
static
void reflections_resources(gapi::CmdEncoder& encoder,
                           const gapi::TextureHandle reflections_target)
{
  encoder.clearColorTexture(reflections_target,
    gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderReadWrite,  {(uint32_t)0}, {});
}
