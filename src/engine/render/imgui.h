#pragma once

#include <imgui/imgui.h>

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

      static void drawAllWindows();

    private:
      static ImGuiGlobalWindowRegistration* m_List;
      ImGuiGlobalWindowRegistration* m_Next;

      const char* m_Name;
      DrawCb m_Cb;
      ImGuiWindowFlags m_Flags;
  };

}

#define IMGUI_REG_WND(name, cb, flags) static const Engine::Render::ImGuiGlobalWindowRegistration imgui_wnd ## __LINE__{name, cb, flags}
