#include <engine/render/frame_graph/dsl.h>
#include <engine/ecs/ecs.h>
#include <engine/debug_marks.h>

NODE_BEGIN(present_producer)
  RENAME_TEX_RT(final_target, present_src)
  NO_EXEC()
NODE_END()

NODE_BEGIN(present)
  READ_TEX_TRANSFER_SRC(present_src)
  MODIFY_TEX_TRANSFER_DST(backbuffer)
  EXEC(present_exec)
NODE_END()

#ifdef DEBUG
  void on_before_present_impl(gapi::CmdEncoder& encoder, gapi::TextureViewWithState& bb)
  {
    GAPI_MARK("before present", encoder);

    Engine::DebugOnBeforePresent evt;
    evt.encoder = &encoder;
    evt.backbuffer = &bb;

    ecs::get_registry()
      .broadcastEventSync(std::move(evt));

    if (bb.getState() != gapi::TextureState::Present)
      bb.transitState(encoder, gapi::TextureState::Present);
  }
#else
  void on_before_present_impl(gapi::CmdEncoder&, gapi::TextureViewWithState&) {}
#endif

static
void on_before_present(gapi::CmdEncoder& encoder)
{
  GAPI_MARK("before present", encoder);

  gapi::TextureViewWithState bb = fg::get_cur_frame_texture("backbuffer");
  on_before_present_impl(encoder, bb);

  if (bb.getState() != gapi::TextureState::Present)
    bb.transitState(encoder, gapi::TextureState::Present);
}

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
  on_before_present(encoder);
}
