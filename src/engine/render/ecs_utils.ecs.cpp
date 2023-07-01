#include "ecs_utils.h"
#include "debug_text_queue.h"

#include <engine/ecs/macros.h>

ECS_QUERY()
static
void query_dbg_queue(eastl::function<void(Engine::Render::dbg::TextDbgQueue& dbg_text_queue)>);

ECS_QUERY()
static
void query_font_render(eastl::function<void(Engine::Render::FontRender& font_render)>);

ECS_QUERY()
static
void query_gui_render(eastl::function<void(Engine::Render::GuiRender& gui_render)>);

ECS_QUERY()
static
void query_imgui_render(eastl::function<void(Engine::Render::ImGuiRender& imgui_render)>);

ECS_QUERY()
static
void query_render_settings(eastl::function<void(const int2 render_window_size)>);

namespace Engine::Render
{
  auto get_font_render() -> FontRender*
  {
     FontRender* fontRender = nullptr;
      query_font_render([&fontRender](FontRender& font_render)
      {
        fontRender = &font_render;
      });

      return fontRender;
  }

  auto get_imgui_render() -> ImGuiRender*
  {
     ImGuiRender* imguiRender = nullptr;
      query_imgui_render([&imguiRender](ImGuiRender& imgui_render)
      {
        imguiRender = &imgui_render;
      });

      return imguiRender;
  }

  auto get_gui_render() -> GuiRender*
  {
     GuiRender* guiRender = nullptr;
      query_gui_render([&guiRender](GuiRender& gui_render)
      {
        guiRender = &gui_render;
      });

      return guiRender;
  }

  auto get_render_size() -> int2
  {
    int2 size{0,0};
    query_render_settings([&](const int2 render_window_size){ size = render_window_size; });
    return size;
  }

  namespace dbg
  {
    auto get_dbg_text_queue() -> TextDbgQueue*
    {
      TextDbgQueue* queue = nullptr;
      query_dbg_queue([&queue](dbg::TextDbgQueue& dbg_text_queue)
      {
        queue = &dbg_text_queue;
      });

      return queue;
    }
  } 
}