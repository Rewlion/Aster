#pragma once

#include <engine/types.h>

namespace Engine::Render
{
  class FontRender;
  class ImGuiRender;
  class GuiRender;

  auto get_font_render() -> FontRender*;
  auto get_imgui_render() -> ImGuiRender*;
  auto get_gui_render() -> GuiRender*;
  auto get_render_size() -> int2;

  namespace dbg
  {
    class TextDbgQueue;

    auto get_dbg_text_queue() -> TextDbgQueue*;
  }
}
