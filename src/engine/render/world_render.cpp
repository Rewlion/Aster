#include "world_render.h"
#include "gui_render.h"

#include <engine/ecs/ecs.h>
#include <engine/render/debug_text_queue.h>
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
      mk_ui_node,
      mk_dbg_text_node,
      mk_present_node
    >(m_FgNodes);

    fg::set_closing_node("present");
  }

  void WorldRender::setupEcs()
  {
    auto& ecs = ecs::get_registry();

    ecs::TemplateComponentsMap dbgTextQueueComps;
    dbgTextQueueComps["dbg_text_queue"] = dbg::TextDbgQueue{};
    ecs.addTemplate("TextDbgQueue", {}, std::move(dbgTextQueueComps));
    ecs.createEntity("TextDbgQueue");

    ecs::TemplateComponentsMap fontRenderComps;
    fontRenderComps["font_render"] = FontRender{};
    ecs.addTemplate("FontRender", {}, std::move(fontRenderComps));
    ecs.createEntity("FontRender");

    ecs::TemplateComponentsMap guiRenderComps;
    guiRenderComps["gui_render"] = GuiRender{};
    ecs.addTemplate("GuiRender", {}, std::move(guiRenderComps));
    ecs.createEntity("GuiRender");

    ecs::TemplateComponentsMap imguiRenderComps;
    imguiRenderComps["imgui_render"] = ImGuiRender{};
    ecs.addTemplate("ImGuiRender", {}, std::move(imguiRenderComps));
    ecs.createEntity("ImGuiRender");

    ecs::TemplateComponentsMap renderSettingsComps;
    renderSettingsComps["render_window_size"] = Window::get_window_size();
    ecs.addTemplate("RenderSettings", {}, std::move(renderSettingsComps));
    ecs.createEntity("RenderSettings");
  }

  void WorldRender::init()
  {
    setupEcs();
    fg::init();
    createFrameGraph();
  }

  void WorldRender::render()
  {
    fg::exec_new_frame();
    gapi::present_backbuffer_and_finalize_frame();
  }
}
