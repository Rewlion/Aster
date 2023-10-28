#include <engine/render/debug/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/gui_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/tfx/tfx.h>
#include <engine/window.h>
#include <engine/events.h>
#include <engine/ecs/macros.h>

ECS_EVENT_SYSTEM()
static
void mk_ui_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("ui", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto finalFrame = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

    reg.requestRenderPass()
       .addTarget(finalFrame, gapi::LoadOp::Load, gapi::StoreOp::Store);

    return [](gapi::CmdEncoder& encoder)
    {
      if (auto* guiRender = Engine::Render::get_gui_render())
        guiRender->render(encoder);

      if (auto* imguiRender = Engine::Render::get_imgui_render())
        imguiRender->render(encoder);
    };
  });
}

ECS_EVENT_SYSTEM()
static
void mk_dbg_text_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("dbg_text", FG_FILE_DECL, [](fg::Registry& reg)
  {
    reg.orderMeAfter("ui");
    auto rt = reg.modifyTexture("final_target", gapi::TextureState::RenderTarget);

    reg.requestRenderPass()
       .addTarget(rt, gapi::LoadOp::Load, gapi::StoreOp::Store);

    return [](gapi::CmdEncoder& encoder)
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
    };
  });
}

ECS_EVENT_SYSTEM()
static
void mk_taa_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("TAA", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto renderSize = Engine::Render::get_render_size();

    gapi::TextureAllocationDescription allocDesc;
    allocDesc.format = gapi::TextureFormat::R8G8B8A8_UNORM;
    allocDesc.extent = int3{renderSize.x, renderSize.y, 1};
    allocDesc.mipLevels = 1;
    allocDesc.arrayLayers = 1;
    allocDesc.usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_SRV | gapi::TEX_USAGE_TRANSFER_SRC;


    auto taaCurrentFrame = reg.readTexture("final_target", gapi::TextureState::ShaderRead);
    auto taaPrevFrame = reg.readTexture("final_antialiased_target", gapi::TextureState::ShaderRead, fg::Timeline::Previous);
    auto motionBuf = reg.readTexture("motionBuf", gapi::TextureState::ShaderRead);
    auto gbufferDepth = reg.readTexture("gbuffer_depth", gapi::TextureState::DepthReadStencilRead);

    auto rt = reg.createTexture("final_antialiased_target", allocDesc, gapi::TextureState::RenderTarget);
    reg.requestRenderPass()
       .addTarget(rt, gapi::LoadOp::DontCare, gapi::StoreOp::Store);

    return [taaCurrentFrame, taaPrevFrame, motionBuf, gbufferDepth](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("taaCurrentFrame", taaCurrentFrame.get());
      tfx::set_extern("taaPrevFrame", taaPrevFrame.get());
      tfx::set_extern("motionBuf", motionBuf.get());
      tfx::set_extern("renderSize", encoder.getRenderSize());
      tfx::set_extern("gbuffer_depth", gbufferDepth.get());
      tfx::activate_technique("TAA", encoder);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });
}

ECS_EVENT_SYSTEM()
static
void mk_present_node(const Engine::OnFrameGraphInit&)
{
  fg::register_node("present", FG_FILE_DECL, [](fg::Registry& reg)
  {
    auto rt = reg.readTexture("final_antialiased_target", gapi::TextureState::TransferSrc);
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
