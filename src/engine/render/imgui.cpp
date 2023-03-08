#include "imgui.h"

#include <engine/assert.h>

#include <imgui/imgui_impl_win32.h>

#include <Windows.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine::Render
{
  ImGuiManager* ImGuiManager::m_Mngr = nullptr;

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

  void ImGuiManager::init()
  {
    if (!m_Mngr)
      m_Mngr = new ImGuiManager();
  }

  void ImGuiManager::destroy()
  {
    if (m_Mngr)
    {
      m_Mngr->~ImGuiManager();
      m_Mngr = nullptr;
    }
  }

  void ImGuiManager::handleWndEvent(void* hwnd,
                                    unsigned int msg,
                                    unsigned long long wParam,
                                    long long param)
  {
    ImGui_ImplWin32_WndProcHandler((HWND)hwnd, msg, wParam, param);
  }

  ImGuiManager::ImGuiManager()
  {
    Window::reg_wnd_proc_handler(this);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(*(void**)Engine::Window::get_hwnd());
  }

  ImGuiManager::~ImGuiManager()
  {
    Window::unreg_wnd_proc_handler(this);
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiManager::tick()
  {
    ASSERT_FMT(m_Mngr, "ImGui Manager is not initialized");
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiGlobalWindowRegistration::drawAllWindows();
    ImGui::Render();
  }
}
