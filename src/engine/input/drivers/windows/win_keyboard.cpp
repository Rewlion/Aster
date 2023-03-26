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

    constexpr size_t maxBtnId = 200;
    if (wParam > maxBtnId)
      return;

    KeyboardButtons key = KeyboardButtons::BKEY_UNKNOWN;
    switch (wParam)
    {
      case VK_BACK:       key = BKEY_BACKSPACE; break;
      case VK_TAB:        key = BKEY_TAB; break;
      case VK_RETURN:     key = BKEY_ENTER; break;
      case VK_SHIFT:
      case VK_MENU:
      case VK_CONTROL:    key = BKEY_EXTRA; break;
      case VK_NUMLOCK:    key = BKEY_NUMLOCK; break;
      case VK_INSERT:     key = BKEY_INSERT; break;
      case 20:            key = BKEY_CAPSLOCK; break;
      case VK_ESCAPE:     key = BKEY_ESCAPE; break;
      case VK_SPACE:      key = BKEY_SPACE; break;
      case 33:            key = BKEY_PAGEUP; break;
      case 34:            key = BKEY_PAGEDOWN; break;
      case VK_END:        key = BKEY_END; break;
      case VK_HOME:       key = BKEY_HOME; break;
      case VK_LEFT:       key = BKEY_LEFT; break;
      case VK_UP:         key = BKEY_UP; break;
      case VK_RIGHT:      key = BKEY_RIGHT; break;
      case VK_DOWN:       key = BKEY_DOWN; break;
      case VK_SELECT:     key = BKEY_SELECT; break;
      case VK_PRINT:      key = BKEY_PRINTSCREEN; break;
      case VK_DELETE:     key = BKEY_DELETE; break;
      case '0':           key = BKEY_0; break;
      case '1':           key = BKEY_1; break;
      case '2':           key = BKEY_2; break;
      case '3':           key = BKEY_3; break;
      case '4':           key = BKEY_4; break;
      case '5':           key = BKEY_5; break;
      case '6':           key = BKEY_6; break;
      case '7':           key = BKEY_7; break;
      case '8':           key = BKEY_8; break;
      case '9':           key = BKEY_9; break;
      case 'A':           key = BKEY_A; break;
      case 'B':           key = BKEY_B; break;
      case 'C':           key = BKEY_C; break;
      case 'D':           key = BKEY_D; break;
      case 'E':           key = BKEY_E; break;
      case 'F':           key = BKEY_F; break;
      case 'G':           key = BKEY_G; break;
      case 'H':           key = BKEY_H; break;
      case 'I':           key = BKEY_I; break;
      case 'J':           key = BKEY_J; break;
      case 'K':           key = BKEY_K; break;
      case 'L':           key = BKEY_L; break;
      case 'M':           key = BKEY_M; break;
      case 'N':           key = BKEY_N; break;
      case 'O':           key = BKEY_O; break;
      case 'P':           key = BKEY_P; break;
      case 'Q':           key = BKEY_Q; break;
      case 'R':           key = BKEY_R; break;
      case 'S':           key = BKEY_S; break;
      case 'T':           key = BKEY_T; break;
      case 'U':           key = BKEY_U; break;
      case 'V':           key = BKEY_V; break;
      case 'W':           key = BKEY_W; break;
      case 'X':           key = BKEY_X; break;
      case 'Y':           key = BKEY_Y; break;
      case 'Z':           key = BKEY_Z; break;
      case VK_LWIN:       key = BKEY_LWIN; break;
      case VK_RWIN:       key = BKEY_RWIN; break;
      case VK_NUMPAD0:    key = BKEY_NUMPAD0; break;
      case VK_NUMPAD1:    key = BKEY_NUMPAD1; break;
      case VK_NUMPAD2:    key = BKEY_NUMPAD2; break;
      case VK_NUMPAD3:    key = BKEY_NUMPAD3; break;
      case VK_NUMPAD4:    key = BKEY_NUMPAD4; break;
      case VK_NUMPAD5:    key = BKEY_NUMPAD5; break;
      case VK_NUMPAD6:    key = BKEY_NUMPAD6; break;
      case VK_NUMPAD7:    key = BKEY_NUMPAD7; break;
      case VK_NUMPAD8:    key = BKEY_NUMPAD8; break;
      case VK_NUMPAD9:    key = BKEY_NUMPAD9; break;
      case VK_MULTIPLY:   key = BKEY_NUMPAD_MULTIPLY; break;
      case VK_ADD:        key = BKEY_NUMPAD_ADD; break;
      case VK_SEPARATOR:  key = BKEY_NUMPAD_SEPARATOR; break;
      case VK_SUBTRACT:   key = BKEY_NUMPAD_SUBTRACT; break;
      case VK_DECIMAL:    key = BKEY_NUMPAD_DECIMAL; break;
      case VK_DIVIDE:     key = BKEY_NUMPAD_DIVIDE; break;
      case VK_F1:         key = BKEY_F1; break;
      case VK_F2:         key = BKEY_F2; break;
      case VK_F3:         key = BKEY_F3; break;
      case VK_F4:         key = BKEY_F4; break;
      case VK_F5:         key = BKEY_F5; break;
      case VK_F6:         key = BKEY_F6; break;
      case VK_F7:         key = BKEY_F7; break;
      case VK_F8:         key = BKEY_F8; break;
      case VK_F9:         key = BKEY_F9; break;
      case VK_F10:        key = BKEY_F10; break;
      case VK_F11:        key = BKEY_F11; break;
      case VK_F12:        key = BKEY_F12; break;
      case VK_F13:        key = BKEY_F13; break;
      case VK_F14:        key = BKEY_F14; break;
      case VK_F15:        key = BKEY_F15; break;
      case VK_F16:        key = BKEY_F16; break;
      case VK_F17:        key = BKEY_F17; break;
      case VK_F18:        key = BKEY_F18; break;
      case VK_F19:        key = BKEY_F19; break;
      case VK_F20:        key = BKEY_F20; break;
      case VK_F21:        key = BKEY_F21; break;
      case VK_F22:        key = BKEY_F22; break;
      case VK_F23:        key = BKEY_F23; break;
      case VK_F24:        key = BKEY_F24; break;
      case VK_OEM_MINUS:  key = BKEY_MINUS; break;
      case VK_OEM_PLUS:   key = BKEY_EQUAL; break;
      case VK_OEM_COMMA:  key = BKEY_COMMA; break;
      case VK_OEM_PERIOD: key = BKEY_PERIOD; break;
      case VK_OEM_3:      key = BKEY_GRAVE_ACCENT; break;
      default: break;
    }

    if (key == BKEY_UNKNOWN) [[unlikely]]
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
