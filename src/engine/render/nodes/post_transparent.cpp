#include "nodes.h"

#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/world_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/render/gui_render.h>

namespace Engine::Render
{
  fg::NodeHandle mk_ui_node()
  {
    return fg::register_node("ui", FG_FILE_DECL, [](fg::Registry& reg)
    {
      auto finalFrame = reg.modifyTexture("final_frame", gapi::TextureState::RenderTarget);

      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);

      return [](gapi::CmdEncoder& encoder)
      {
        world_render.m_GuiRender->render(encoder);
        world_render.m_ImGuiRender->render(encoder);
      };
    });
  }

  fg::NodeHandle mk_dbg_text_node()
  {
    return fg::register_node("dbg_text", FG_FILE_DECL, [](fg::Registry& reg)
    {
      reg.orderMeAfter("ui");
      auto finalFrame = reg.modifyTexture("final_frame", gapi::TextureState::RenderTarget);
      reg.requestRenderPass()
         .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);
      return [](gapi::CmdEncoder& encoder)
      {
        world_render.renderDbgText(encoder);
      };
    });
  }

  fg::NodeHandle mk_present_node()
  {
    return fg::register_node("present", FG_FILE_DECL, [](fg::Registry& reg)
    {
      auto finalFrame = reg.readTexture("final_frame", gapi::TextureState::TransferSrc);
      auto backbuffer = reg.modifyTexture("backbuffer", gapi::TextureState::TransferDst);
      return [finalFrame, backbuffer](gapi::CmdEncoder& encoder)
      {
        const auto region = gapi::TextureSubresourceLayers{
          .aspects = gapi::ASPECT_COLOR,
          .mipLevel = 0,
          .baseArrayLayer = 0,
          .layerCount = 1
        };

        const uint3 finalFrameExtent = finalFrame.describe().extent;

        const auto blit = gapi::TextureBlit{
          .srcSubresource = region,
          .srcOffsets = {int3{0,0,0}, finalFrameExtent},
          .dstSubresource = region,
          .dstOffsets = {int3{0,0,0}, finalFrameExtent},
        };

        encoder.blitTexture(finalFrame.get(), backbuffer.get(), 1, &blit, gapi::ImageFilter::Nearest);
        encoder.transitTextureState(backbuffer.get(), gapi::TextureState::TransferDst, gapi::TextureState::Present);
      };
    });
  }

}
