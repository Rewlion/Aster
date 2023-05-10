#include "imgui.h"

#include <engine/assert.h>
#include <engine/time.h>

#include <imgui-node-editor/imgui_node_editor.h>

#include <Windows.h>

namespace ed = ax::NodeEditor;

namespace Engine::imgui
{
  Manager* Manager::m_This = nullptr;

  ImGuiGlobalWindowRegistration* ImGuiGlobalWindowRegistration::m_List = nullptr;
  ImGuiGlobalWindowRegistration::ImGuiGlobalWindowRegistration(DrawCb cb)
  {
    m_Next = m_List;
    m_List = this;

    m_Cb = cb;
  }

  void ImGuiGlobalWindowRegistration::drawAllWindows()
  {
    const ImGuiGlobalWindowRegistration* p = m_List;
    while(p)
    {
      p->m_Cb();
      p = p->m_Next;
    }
  }

  void Manager::init()
  {
    if (!m_This)
      m_This = new Manager();
  }

  void Manager::destroy()
  {
    if (m_This)
    {
      m_This->~Manager();
      m_This = nullptr;
    }
  }

  Manager::Manager()
  {
    Input::Router::registerListener(this);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "engine win64";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    m_State = State::Disabled;

    m_NodeEditorContext = ed::CreateEditor(nullptr);
  }

  Manager::~Manager()
  {
    Input::Router::unregisterListener(this);

    ed::DestroyEditor(m_NodeEditorContext);

    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;

    ImGui::DestroyContext();
  }

  void Manager::tick()
  {
    ASSERT_FMT(m_This, "ImGui Manager is not initialized");
    
    ImGuiIO& io = ImGui::GetIO();
    const float2 wndSize = Window::get_window_size();
    io.DisplaySize = ImVec2(wndSize.x, wndSize.y);
    io.DeltaTime = Time::get_dt();

    ImGui::NewFrame();
    ed::SetCurrentEditor(m_This->m_NodeEditorContext);
    ImGuiGlobalWindowRegistration::drawAllWindows();
    ImGui::Render();
  }
}
