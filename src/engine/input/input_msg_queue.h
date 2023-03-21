#pragma once

#include "input_device_type.h"

#include <engine/types.h>
#include <engine/utils/spinlock.h>

#include <EASTL/vector.h>

#include <variant>

namespace Engine::Input
{
  struct ButtonMsgInput
  {
    int key;
    bool pressed;
    InputDevice device;
  };

  struct MouseMoveMsgInput
  {
    int2 newPos;
    int2 posDelta;
  };

  using InputMsg = std::variant<ButtonMsgInput, MouseMoveMsgInput>;

  class InputMsgQueue
  {
    public:
      void addButtonInput(const ButtonMsgInput& data)
      {
        Utils::ScopeSpinlock l{m_Spinlock};
        m_Messages.push_back() = data;
      }

      void addMouseMoveInput(const MouseMoveMsgInput& data)
      {
        Utils::ScopeSpinlock l{m_Spinlock};
        m_Messages.push_back() = data;
      }

      template<class Cb>
      void processMessages(Cb cb)
      {
        Utils::ScopeSpinlock l{m_Spinlock};
        for (const auto& msg: m_Messages)
          cb(msg);
        m_Messages.clear();
      }

    private:
      Utils::Spinlock m_Spinlock;
      eastl::vector<InputMsg> m_Messages;
  };
}
