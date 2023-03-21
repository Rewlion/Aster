#include "win_keyboard.h"

#include <engine/window.h>

namespace Engine::Input
{
  auto create_keyboard_driver() -> IKeyboardDriver*
  {
    return new WinKeyboardDriver;
  }

  auto WinKeyboard::setButtonState(const int key, const bool state) -> bool
  {
    const bool changed = m_Buttons.isSet(key) != state;
    m_Buttons.set(key, state);
    return changed;
  }

  auto WinKeyboard::isButtonPressed(const int key) const -> bool
  {
    return m_Buttons.isSet(key);
  }

  WinKeyboardDriver::WinKeyboardDriver()
  {
    Window::reg_wnd_proc_handler(this);
  }

  WinKeyboardDriver::~WinKeyboardDriver()
  {
    Window::unreg_wnd_proc_handler(this);
  }
  
  void WinKeyboardDriver::handleWndEvent(void* hwnd,
                                         unsigned int msg,
                                         unsigned long long wParam,
                                         long long lParam)
  {
    if (!(msg & Window::WND_MSG_KEY_MASK))
      return;

    constexpr size_t maxBtnId = 136;
    if (wParam > maxBtnId)
      return;

    const int btnRemap[maxBtnId] = {
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      BKEY_BACKSPACE,
      BKEY_TAB,
      0,
      0,
      0,
      BKEY_ENTER,
      0,
      0,
      BKEY_EXTRA,
      BKEY_EXTRA,
      BKEY_EXTRA,
      0,
      BKEY_CAPSLOCK,
      0,
      0,
      0,
      0,
      0,
      0,
      BKEY_ESCAPE,
      0,
      0,
      0,
      0,
      BKEY_SPACE   ,
      BKEY_PAGEUP  ,
      BKEY_PAGEDOWN,
      BKEY_END     ,
      BKEY_HOME    ,
      BKEY_LEFT    ,
      BKEY_UP      ,
      BKEY_RIGHT   ,
      BKEY_DOWN    ,
      BKEY_SELECT  ,
      BKEY_PRINT   ,
      0,
      BKEY_PRINTSCREEN,
      0,
      0,
      0,
      BKEY_0,
      BKEY_1,
      BKEY_2,
      BKEY_3,
      BKEY_4,
      BKEY_5,
      BKEY_6,
      BKEY_7,
      BKEY_8,
      BKEY_9,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      BKEY_A,
      BKEY_B,
      BKEY_C,
      BKEY_D,
      BKEY_E,
      BKEY_F,
      BKEY_G,
      BKEY_H,
      BKEY_I,
      BKEY_J,
      BKEY_K,
      BKEY_L,
      BKEY_M,
      BKEY_N,
      BKEY_O,
      BKEY_P,
      BKEY_Q,
      BKEY_R,
      BKEY_S,
      BKEY_T,
      BKEY_U,
      BKEY_V,
      BKEY_W,
      BKEY_X,
      BKEY_Y,
      BKEY_Z,
      BKEY_LWIN,
      BKEY_RWIN,
      0,
      0,
      0,
      BKEY_NUMPAD0,
      BKEY_NUMPAD1,
      BKEY_NUMPAD2,
      BKEY_NUMPAD3,
      BKEY_NUMPAD4,
      BKEY_NUMPAD5,
      BKEY_NUMPAD6,
      BKEY_NUMPAD7,
      BKEY_NUMPAD8,
      BKEY_NUMPAD9,
      BKEY_MULTIPLY,
      BKEY_ADD,
      BKEY_SEPARATOR,
      BKEY_SUBTRACT,
      BKEY_DECIMAL,
      BKEY_DIVIDE,
      BKEY_F1,
      BKEY_F2,
      BKEY_F3,
      BKEY_F4,
      BKEY_F5,
      BKEY_F6,
      BKEY_F7,
      BKEY_F8,
      BKEY_F9,
      BKEY_F10,
      BKEY_F11,
      BKEY_F12,
      BKEY_F13,
      BKEY_F14,
      BKEY_F15,
      BKEY_F16,
      BKEY_F17,
      BKEY_F18,
      BKEY_F19,
      BKEY_F20,
      BKEY_F21,
      BKEY_F22,
      BKEY_F23,
      BKEY_F24
    };

    KeyboardButtons key = (KeyboardButtons)btnRemap[wParam];
    if (key == BKEY_UNKNOWN)
      return;

    if (key == BKEY_EXTRA)
    {
      switch (wParam)
      {
        case VK_SHIFT: key = lParam & KF_EXTENDED ? BKEY_RSHIFT : BKEY_LSHIFT; break;
        case VK_CONTROL: key = lParam & KF_EXTENDED ? BKEY_RCTRL : BKEY_LCTRL; break;
        case VK_MENU: key = lParam & KF_EXTENDED ? BKEY_RALT : BKEY_LALT; break;
      }
    }
    
    const bool pressed = msg & Window::WND_MSG_KEY_DOWN;
    const bool stateChanged = m_Kb.setButtonState(key, pressed);

    if (m_Listener && stateChanged)
      m_Listener->onKeyStateChanged(m_Kb, key, pressed);
  }
}
