#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(present_producer)
  RENAME_TEX_RT(final_antialiased_target, present_src)
  NO_EXEC()
NODE_END()

NODE_BEGIN(present)
  READ_TEX_TRANSFER_SRC(present_src)
  MODIFY_TEX_TRANSFER_DST(backbuffer)
  EXEC(present_exec)
NODE_END()

NODE_EXEC()
static
void present_exec(gapi::CmdEncoder& encoder,
                  const gapi::TextureHandle present_src,
                  const gapi::TextureHandle backbuffer)
{
  const auto region = gapi::TextureSubresourceLayers{
    .aspects = gapi::ASPECT_COLOR,
    .mipLevel = 0,
    .baseArrayLayer = 0,
    .layerCount = 1
  };

  const uint3 rtFrameExtent = gapi::get_texture_extent(present_src);

  const auto blit = gapi::TextureBlit{
    .srcSubresource = region,
    .srcOffsets = {int3{0,0,0}, rtFrameExtent},
    .dstSubresource = region,
    .dstOffsets = {int3{0,0,0}, rtFrameExtent},
  };

  encoder.blitTexture(present_src, backbuffer, 1, &blit, gapi::ImageFilter::Nearest);
  encoder.transitTextureState(backbuffer, gapi::TextureState::TransferDst, gapi::TextureState::Present);
}
