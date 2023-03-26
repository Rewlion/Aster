#include "editor.h"

#include <engine/assert.h>
#include <engine/console/console.h>
#include <engine/input/drivers/buttons.h>
#include <engine/utils/bits.h>

namespace Engine::editor
{
  Manager* Manager::m_This = nullptr;

  void Manager::init()
  {
    ASSERT(m_This == nullptr);
    m_This = new Manager();
  }

  void Manager::destroy()
  {
    ASSERT(m_This != nullptr);

    m_This->~Manager();
    m_This = nullptr;
  }

  Manager::Manager()
  {
    Input::Router::registerListener(this);
    m_State = STATE_DISABLED;
  }

  Manager::~Manager()
  {
    Input::Router::unregisterListener(this);
  }

  auto Manager::onKeyStateChanged(const Input::IKeyboard& device,
                                  const int key,
                                  const bool pressed) -> Input::InputRouterProcessStatus
  {
    const int oldState = m_State;

    if (key == Input::BKEY_GRAVE_ACCENT && pressed)
    {
      Utils::toggle_bit(m_State, STATE_CONSOLE_BIT);
    }

    const bool changed = oldState != m_State;

    if (changed)
    {
      if (m_State & IMGUI_ACTIVE_STATE_MASK)
      {
        setImGuiState(imgui::Manager::State::ShowAndConsumeInput);
        Window::show_cursor(true);
        console::enable(true);
      }
      else
      {
        setImGuiState(imgui::Manager::State::Disabled);
        Window::show_cursor(false);
        console::enable(false);
      }

      return Input::InputRouterProcessStatus::StopProcessing;
    }

    return Input::InputRouterProcessStatus::ProcessFurther;
  }
}
