#pragma once

#include <imgui/imgui.h>

#include <engine/window.h>
#include <engine/input/input_router.h>

namespace Engine::imgui
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

  class Manager: public Input::IInputRouterListener
  {
    public:
      auto getInputRouterPriority() -> int override { return 0; }

      auto onMouseButtonStateChanged(const Input::IPointer& device,
                                     const int key, 
                                     const bool pressed) -> Input::InputRouterProcessStatus override;

      auto onMouseMove(const Input::IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> Input::InputRouterProcessStatus override;

      auto onKeyStateChanged(const Input::IKeyboard& device,
                             const int key,
                             const bool pressed) -> Input::InputRouterProcessStatus override;

      static
      void init();

      static
      void destroy();

      static
      void tick();

    private:
      Manager();
      ~Manager();

    private:
      static Manager* m_Mngr;
  };

}

#define IMGUI_REG_WND(name, cb, flags) static const Engine::imgui::ImGuiGlobalWindowRegistration imgui_wnd ## __LINE__{name, cb, flags}
