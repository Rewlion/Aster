#include <engine/console/cmd.h>
#include <engine/console/console.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/tfx/tfx.h>

void register_show_tex_fg_node(eastl::span<string_view> args)
{
  string tex = string(args[0]);
  fg::register_node("dbg_show_tex", FG_FILE_DECL, [tex = std::move(tex)](fg::Registry& reg)
  {
    auto texHndl = reg.readTexture(tex.c_str(), gapi::TextureState::ShaderRead);
    reg.requestRenderPass()
       .addTarget("final_antialiased_target");

    auto wndSize = reg.readBlob<int2>("window_size"); 
      
    return [texHndl, wndSize](gapi::CmdEncoder& encoder)
    {
      tfx::set_extern("texToShow", texHndl.get());
      tfx::activate_technique("ShowTex", encoder);

      float vpSize = (float)wndSize->y * 0.3;
      gapi::Viewport vp {
        .pos = {0,0},
        .size = {vpSize,vpSize}
      };
      encoder.setViewport(vp);

      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    };
  });
}

CONSOLE_CMD("render.show_fg_tex", 1, 1, register_show_tex_fg_node);