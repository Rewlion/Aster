#include <engine/render/frame_graph/dsl.h>

#include <engine/render/ecs_utils.h>
#include <engine/render/gui_render.h>
#include <engine/render/imgui/imgui_render.h>

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
