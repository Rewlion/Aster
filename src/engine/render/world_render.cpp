#include "world_render.h"
#include "gui_render.h"

#include <engine/algorithm/hash.h>
#include <engine/assets/assets_manager.h>
#include <engine/gapi/gapi.h>
#include <engine/log.h>
#include <engine/math.h>
#include <engine/render/font/font_render.h>
#include <engine/render/imgui/imgui_render.h>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/scene/scene.h>
#include <engine/settings.h>
#include <engine/time.h>
#include <engine/window.h>
#include <engine/render/nodes/nodes.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


namespace Engine::Render
{
  WorldRender world_render;

  using NodeConstructor = fg::NodeHandle(*)();
  template<NodeConstructor ...createNodes>
  void push_back_to(eastl::vector<fg::NodeHandle>& to)
  {
    (to.push_back(createNodes()), ...);
  }

  void WorldRender::createFrameGraph()
  {
    push_back_to<
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

  void WorldRender::init()
  {
    fg::init();
    createFrameGraph();

    m_WindowSize = Window::get_window_size();
    m_Aspect = (float)m_WindowSize.x / (float)m_WindowSize.y;

    gapi::SamplerAllocationDescription samplerAllocDesc;
    m_ModelSampler = allocate_sampler(samplerAllocDesc);

    m_FontRender = std::make_unique<FontRender>();
    m_GuiRender = std::make_unique<GuiRender>(*m_FontRender.get());
    m_ImGuiRender = std::make_unique<ImGuiRender>();
    m_ImGuiRender->init();

    const DataBlock* settings = Engine::get_app_settings();
    const string fontFolder = settings->getText("fonts_folder");
    if (fontFolder != "")
    {
      const string font = fontFolder + "/arial.ttf";
      m_FontRender->init(font);
    }
    else
      logerror("world_render: missing font folder inside settings");
  }

  void WorldRender::render(const CameraData& cameraVP)
  {
    m_FrameData.camera = cameraVP;
    fg::exec_new_frame();
    gapi::present_backbuffer_and_finalize_frame();
  }

  void WorldRender::renderDbgText(gapi::CmdEncoder& encoder)
  {
    const float textSize = m_WindowSize.y * 0.03;
    float i = 0;
    for (const auto& e: m_DbgTextQueue)
    {
      const float2 pos = {5.0f, textSize * i};
      m_FontRender->render(e.text, pos, textSize, e.color, encoder);
      ++i;
    }
  }
}
