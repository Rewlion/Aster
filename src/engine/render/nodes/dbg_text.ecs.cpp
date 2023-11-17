#include <engine/render/debug/debug_text_queue.h>
#include <engine/render/ecs_utils.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/dsl.h>
#include <engine/window.h>

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
