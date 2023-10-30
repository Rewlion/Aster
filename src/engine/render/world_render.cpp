#include "world_render.h"
#include "gui_render.h"

#include <engine/ecs/ecs.h>
#include <engine/events.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/window.h>

namespace Engine::Render
{
  WorldRender world_render;

  void WorldRender::createFrameGraph()
  {
    ecs::get_registry().broadcastEventSync(OnFrameGraphInit{});
    fg::set_closing_node("present");
  }

  void WorldRender::setupEcs()
  {
    auto& ecs = ecs::get_registry();

    auto init = ecs::EntityComponents{};
    init["render_window_size"] = Window::get_window_size();

    ecs.createEntity("CoreRenderDebug", std::move(init));
  }

  void WorldRender::init()
  {
    setupEcs();
    fg::init();
    createFrameGraph();
  }

  void WorldRender::render()
  {
    fg::set_render_size(Window::get_window_size());
    fg::exec_new_frame();
    gapi::present_backbuffer_and_finalize_frame();
  }
}
