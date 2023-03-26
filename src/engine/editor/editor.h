#pragma once

#include <engine/render/imgui/imgui.h>

namespace Engine::editor
{
  class Manager: public Input::IInputRouterListener, imgui::Manager::StateChanger
  {
    public:
      int getInputRouterPriority() override { return 0; };

      auto onKeyStateChanged(const Input::IKeyboard& device,
                             const int key,
                             const bool pressed) -> Input::InputRouterProcessStatus override;

      static
      void init();

      static
      void destroy();

    private:
      Manager();
      ~Manager();

    private:
      static Manager* m_This;

      enum State
      {
        STATE_DISABLED = 0,

        STATE_CONSOLE  = 1,
        STATE_CONSOLE_BIT = 0,

        IMGUI_ACTIVE_STATE_MASK = STATE_CONSOLE
      };
      State m_State;
  };
}
