#pragma once

#include <imgui/imgui.h>

#include <engine/window.h>

namespace Engine::Render
{
  class ImGuiGlobalWindowRegistration
  {
    friend class Registry;
    public:
      using DrawCb = void(*)();
      ImGuiGlobalWindowRegistration(const char* name,
                                    DrawCb cb,
                                    ImGuiWindowFlags flags);

      static
      void drawAllWindows();

    private:
      static ImGuiGlobalWindowRegistration* m_List;
      ImGuiGlobalWindowRegistration* m_Next;

      const char* m_Name;
      DrawCb m_Cb;
      ImGuiWindowFlags m_Flags;
  };

  class ImGuiManager: public Window::IWndProcHandler
  {
    public:
      static
      void init();

      static
      void destroy();

      virtual
      void handleWndEvent(void* hwnd,
                          unsigned int msg,
                          unsigned long long wParam,
                          long long param) override;

      static void tick();

    private:
      ImGuiManager();
      ~ImGuiManager();

    private:
      static ImGuiManager* m_Mngr;
  };

}

#define IMGUI_REG_WND(name, cb, flags) static const Engine::Render::ImGuiGlobalWindowRegistration imgui_wnd ## __LINE__{name, cb, flags}
