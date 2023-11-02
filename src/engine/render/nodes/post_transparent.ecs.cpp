#include <engine/render/debug/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/render/gui_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>

NODE_BEGIN(ui)
  RP_BEGIN()
    TARGET(final_target)
  RP_END()
  EXEC(ui_exec)
NODE_END()

NODE_EXEC()
static
void ui_exec(gapi::CmdEncoder& encoder)
{
  if (auto* guiRender = Engine::Render::get_gui_render())
    guiRender->render(encoder);

  if (auto* imguiRender = Engine::Render::get_imgui_render())
    imguiRender->render(encoder);
}

NODE_BEGIN(dbg_text)
  ORDER_ME_AFTER(ui)
  RP_BEGIN()
    TARGET(final_target)
  RP_END()
  EXEC(dbg_text_exec)
NODE_END()

NODE_EXEC()
static
void dbg_text_exec(gapi::CmdEncoder& encoder)
{
  auto* fontRender = Engine::Render::get_font_render();
  auto* dbgTextQueue = Engine::Render::dbg::get_dbg_text_queue();

  if (fontRender && dbgTextQueue)
  {
    const float textSize = Engine::Window::get_window_size().y * 0.03;
    float i = 0;
    for (const auto& e: *dbgTextQueue)
    {
      const float2 pos = {5.0f, textSize * i};
      fontRender->render(e.text, pos, textSize, e.color, encoder);
      ++i;
    }
  }
}

NODE_BEGIN(TAA)
  READ_TEX_AS(final_target, AS(taa_current_frame), TEX_STATE(ShaderRead))
  READ_TEX_PREV_FRAME_AS(final_antialiased_target, AS(taa_prev_frame), TEX_STATE(ShaderRead))
  READ_TEX(motionBuf, TEX_STATE(ShaderRead))
  READ_TEX(gbuffer_depth, TEX_STATE(DepthReadStencilRead))
  CREATE_TEX_2D(final_antialiased_target, TEX_SIZE_RELATIVE(), R8G8B8A8_UNORM, TEX_USAGE3(RT,SRV,TRANSFER_SRC), TEX_STATE(RenderTarget))

  RP_BEGIN()
    TARGET_LOAD_DONTCARE(final_antialiased_target)
  RP_END()

  EXEC(taa_exec)
NODE_END()

NODE_EXEC()
static
void taa_exec(const gapi::TextureHandle taa_current_frame,
              const gapi::TextureHandle taa_prev_frame,
              const gapi::TextureHandle motionBuf,
              const gapi::TextureHandle gbuffer_depth,
              gapi::CmdEncoder& encoder)
{
  tfx::set_extern("taaCurrentFrame", taa_current_frame);
  tfx::set_extern("taaPrevFrame", taa_prev_frame);
  tfx::set_extern("motionBuf", motionBuf);
  tfx::set_extern("renderSize", encoder.getRenderSize());
  tfx::set_extern("gbuffer_depth", gbuffer_depth);
  tfx::activate_technique("TAA", encoder);

  encoder.updateResources();
  encoder.draw(4, 1, 0, 0);
}

NODE_BEGIN(present)
  READ_TEX(final_antialiased_target, TEX_STATE(TransferSrc))
  MODIFY_TEX(backbuffer, TEX_STATE(TransferDst))
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
