#pragma once

#include <engine/window.h>
#include <engine/input/input_router.h>

#include <imgui/imgui.h>

namespace ax::NodeEditor
{
  struct EditorContext;
}

namespace Engine::imgui
{
  class ImGuiGlobalWindowRegistration
  {
    friend class Registry;
    public:
      using DrawCb = void(*)();
      ImGuiGlobalWindowRegistration(DrawCb cb);

      static
      void drawAllWindows();

    private:
      static ImGuiGlobalWindowRegistration* m_List;
      ImGuiGlobalWindowRegistration* m_Next;

      DrawCb m_Cb;
  };

  class Manager: public Input::IInputRouterListener
  {
    public:
      enum class State
      {
        Disabled,
        ShowAndConsumeInput,
        ShowOnly,
        Last
      };

      class StateChanger
      {
        public:
          void setImGuiState(State state) { Manager::setState(state); }
      };

    public:
      auto getInputRouterPriority() -> int override { return 1; }

      auto onMouseButtonStateChanged(const Input::IPointer& device,
                                     const int key, 
                                     const bool pressed) -> Input::InputRouterProcessStatus override;

      auto onMouseMove(const Input::IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> Input::InputRouterProcessStatus override;

      auto onMouseWheelScroll(const Input::IPointer& device, const float delta)
        -> Input::InputRouterProcessStatus override;

      auto onKeyStateChanged(const Input::IKeyboard& device,
                             const int key,
                             const bool pressed) -> Input::InputRouterProcessStatus override;

      static
      void init();

      static
      void destroy();

      static
      void tick();

      static
      auto getState() -> State { return m_This->m_State; }

    private:
      Manager();
      ~Manager();

      static
      void setState(State state) { m_This->m_State = state; }

    private:
      static Manager* m_This;

      State m_State;
      ax::NodeEditor::EditorContext* m_NodeEditorContext;
  };

}

#define IMGUI_REG_WND(cb) static const Engine::imgui::ImGuiGlobalWindowRegistration imgui_wnd ## __LINE__{cb}
