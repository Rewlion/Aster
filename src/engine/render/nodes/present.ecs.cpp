#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(present)
  READ_TEX_TRANSFER_SRC(final_antialiased_target)
  MODIFY_TEX_TRANSFER_DST(backbuffer)
  EXEC(present_exec)
NODE_END()

NODE_EXEC()
static
void present_exec(gapi::CmdEncoder& encoder,
                  const gapi::TextureHandle final_antialiased_target,
                  const gapi::TextureHandle backbuffer)
{
  const auto region = gapi::TextureSubresourceLayers{
    .aspects = gapi::ASPECT_COLOR,
    .mipLevel = 0,
    .baseArrayLayer = 0,
    .layerCount = 1
  };

  const uint3 rtFrameExtent = gapi::get_texture_extent(final_antialiased_target);

  const auto blit = gapi::TextureBlit{
    .srcSubresource = region,
    .srcOffsets = {int3{0,0,0}, rtFrameExtent},
    .dstSubresource = region,
    .dstOffsets = {int3{0,0,0}, rtFrameExtent},
  };

  encoder.blitTexture(final_antialiased_target, backbuffer, 1, &blit, gapi::ImageFilter::Nearest);
  encoder.transitTextureState(backbuffer, gapi::TextureState::TransferDst, gapi::TextureState::Present);
}
