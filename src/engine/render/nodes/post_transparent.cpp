#include "nodes.h"

#include <engine/render/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/gui_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/window.h>

namespace Engine::Render
{
  fg::node_id_t mk_ui_node()
  {
    return fg::register_node("ui", FG_FILE_DECL, [](fg::Registry& reg)
    {
      auto finalFrame = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);

      return [](gapi::CmdEncoder& encoder)
      {
        if (auto* guiRender = get_gui_render())
          guiRender->render(encoder);

        if (auto* imguiRender = get_imgui_render())
          imguiRender->render(encoder);
      };
    });
  }

  fg::node_id_t mk_dbg_text_node()
  {
    return fg::register_node("dbg_text", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.orderMeAfter("ui");
      auto rt = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

      reg.requestRenderPass()
         .addTarget(rt, gapi::LoadOp::Load, gapi::StoreOp::Store);

      return [](gapi::CmdEncoder& encoder)
      {
        auto* fontRender = get_font_render();
        auto* dbgTextQueue = dbg::get_dbg_text_queue();

        if (fontRender && dbgTextQueue)
        {
          const float textSize = Window::get_window_size().y * 0.03;
          float i = 0;
          for (const auto& e: *dbgTextQueue)
          {
            const float2 pos = {5.0f, textSize * i};
            fontRender->render(e.text, pos, textSize, e.color, encoder);
            ++i;
          }
        }
      };
    });
  }

  fg::node_id_t mk_present_node()
  {
    return fg::register_node("present", FG_FILE_DECL, [](fg::Registry& reg)
    {
      auto rt = reg.readTexture("final_target", gapi::TextureState::TransferSrc);
      auto backbuffer = reg.modifyTexture("backbuffer", gapi::TextureState::TransferDst);
      return [rt, backbuffer](gapi::CmdEncoder& encoder)
      {
        const auto region = gapi::TextureSubresourceLayers{
          .aspects = gapi::ASPECT_COLOR,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1
        };

        const uint3 rtFrameExtent = rt.describe().extent;

        const auto blit = gapi::TextureBlit{
          .srcSubresource = region,
          .srcOffsets = {int3{0,0,0}, rtFrameExtent},
          .dstSubresource = region,
          .dstOffsets = {int3{0,0,0}, rtFrameExtent},
        };

        encoder.blitTexture(rt.get(), backbuffer.get(), 1, &blit, gapi::ImageFilter::Nearest);
        encoder.transitTextureState(backbuffer.get(), gapi::TextureState::TransferDst, gapi::TextureState::Present);
      };
    });
  }

}
