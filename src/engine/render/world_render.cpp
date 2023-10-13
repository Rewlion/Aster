#include "world_render.h"
#include "gui_render.h"

#include <engine/ecs/ecs.h>
#include <engine/render/font/font_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/render/nodes/nodes.h>
#include <engine/window.h>

namespace Engine::Render
{
  WorldRender world_render;

  using NodeConstructor = fg::node_id_t(*)();
  template<NodeConstructor ...createNodes>
  void emplace_back_to(eastl::vector<fg::NodeHandle>& to)
  {
    (to.emplace_back(createNodes()), ...);
  }

  void WorldRender::createFrameGraph()
  {
    emplace_back_to<
      mk_backbuffer_acquiring_node,
      mk_frame_preparing_node,
      mk_gbuffer_main_pass_node,
      mk_gbuffer_resolve_node,
      mk_decals_node,
      mk_early_transparent_sync_node,
      mk_transparent_sync_node,
      mk_post_process_node,
      mk_taa_node,
      mk_ui_node,
      mk_dbg_text_node,
      mk_present_node
    >(m_FgNodes);

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
    fg::init();
    createFrameGraph();
    setupEcs();
  }

  void WorldRender::render()
  {
    fg::exec_new_frame();
    gapi::present_backbuffer_and_finalize_frame();
  }
}
