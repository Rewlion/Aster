#include <engine/console/cmd.h>
#include <engine/console/console.h>
#include <engine/tfx/tfx.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/macros.h>
#include <engine/events_render.h>
#include <engine/render/frame_graph/frame_graph.h>

namespace
{
  struct Params
  {
    string texToShow;
    std::optional<float2> viewportSize;
    std::optional<float> relativeViewport;
  };
  
  Params params;

  float2 get_viewport_size(const float2 render_size)
  {
    if (params.relativeViewport.has_value())
      return render_size * params.relativeViewport.value();
    else if (params.viewportSize.has_value())
      return params.viewportSize.value();

    return render_size * 0.3f;
  }
}

void show_tex(eastl::span<string_view> args)
{
  params = {};

  if (args.size() == 1)
  {
    if (args[0] == "help")
      console::clog("usage: <cmd> <fg_tex> | <cmd> <fg_tex> <relative_size> | <cmd> <fg_tex> <viewport_width> <viewport_height>");
    else
      params.texToShow = args[0];
  }
  else if (args.size() == 2)
  {
    params.texToShow = args[0];
    params.relativeViewport = std::atof(args[1].data());
  }
  else if (args.size() == 3)
  {
    params.texToShow = args[0];
    params.viewportSize = float2{std::atof(args[1].data()),
                                 std::atof(args[2].data())};
  }
}

ECS_EVENT_SYSTEM()
static
void render_tex_on_bb(Engine::DebugOnBeforePresent& evt)
{
  if (params.texToShow.empty())
    return;

  gapi::CmdEncoder& encoder = *evt.encoder;

  gapi::TextureViewWithState* fgTex = fg::get_cur_frame_texture(params.texToShow.c_str());
  if (!fgTex || (gapi::TextureHandle)*fgTex == gapi::TextureHandle::Invalid)
  {
    console::clogerror(fmt::format("unknown texture `{}`", params.texToShow));
    params.texToShow = "";
    return;
  }
  
  if (fgTex->getState() != gapi::TextureState::ShaderRead)
    fgTex->transitState(encoder, gapi::TextureState::ShaderRead);

  if (evt.backbuffer->getState() != gapi::TextureState::RenderTarget)
    evt.backbuffer->transitState(encoder, gapi::TextureState::RenderTarget);

  encoder.beginRenderpass({gapi::RenderPassAttachment{*evt.backbuffer}}, {});
  {
    const float2 renderSize = encoder.getRenderSize();
    tfx::set_extern("texToShow", (gapi::TextureHandle)*fgTex);
    tfx::activate_technique("ShowTex", encoder);

    gapi::Viewport vp {
      .pos = {0,0},
      .size = get_viewport_size(renderSize)
    };
    encoder.setViewport(vp);

    encoder.updateResources();
    encoder.draw(4, 1, 0, 0);
  }
  encoder.endRenderpass();
}


//shows FRAME GRAPHS's texture of the current frame
CONSOLE_CMD("render.show_tex", 0, 3, show_tex);