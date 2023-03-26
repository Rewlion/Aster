#include "imgui.h"

#include <engine/assert.h>
#include <engine/time.h>

#include <Windows.h>

namespace Engine::imgui
{
  Manager* Manager::m_This = nullptr;

  ImGuiGlobalWindowRegistration* ImGuiGlobalWindowRegistration::m_List = nullptr;
  ImGuiGlobalWindowRegistration::ImGuiGlobalWindowRegistration(const char* name,
                                                               DrawCb cb,
                                                               ImGuiWindowFlags flags)
  {
    m_Next = m_List;
    m_List = this;

    m_Name = name;
    m_Cb = cb;
    m_Flags = flags;
  }

  void ImGuiGlobalWindowRegistration::drawAllWindows()
  {
    const ImGuiGlobalWindowRegistration* p = m_List;
    while(p)
    {
      ImGui::Begin(p->m_Name, nullptr, p->m_Flags);
      p->m_Cb();
      ImGui::End();

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
  }

  Manager::~Manager()
  {
    Input::Router::unregisterListener(this);

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
    ImGuiGlobalWindowRegistration::drawAllWindows();
    ImGui::Render();
  }
}
