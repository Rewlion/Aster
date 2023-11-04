#include "win_mouse.h"

#include <Windows.h>
#include <Windowsx.h>
#include <WinUser.h>

namespace Engine::Input
{
  auto create_pointer_driver() -> IPointerDriver*
  {
    return new WinMouseDriver;
  }

  auto WinMouse::setButtonState(const int key, const bool state) -> bool
  {
    const int mouseBtn = key;
    const bool changed = m_Buttons.isSet(mouseBtn) != state;
    m_Buttons.set(mouseBtn, state);
    return changed;
  }

  auto WinMouse::isButtonPressed(const int key) const -> bool
  {
    const int mouseBtn = key;
    return m_Buttons.isSet(mouseBtn);
  }

  auto WinMouse::setPointerPos(const int2 pos) -> bool
  {
    const bool stateChanged = m_PointerPos != pos;
    m_PointerPos = pos;
    return stateChanged;
  }

  WinMouseDriver::WinMouseDriver()
  {
    Window::reg_wnd_proc_handler(this);
    m_Listener = nullptr;

    POINT pos {0,0};
    GetCursorPos(&pos);
    HWND hwnd = (HWND)Window::get_hwnd();
    ScreenToClient(hwnd, &pos);
    m_Ms.setPointerPos(int2{pos.x, pos.y});
  }

  WinMouseDriver::~WinMouseDriver()
  {
    Window::unreg_wnd_proc_handler(this);
  }

  void WinMouseDriver::handleWndEvent(void* hwnd,
                                      unsigned int msg,
                                      unsigned long long wParam,
                                      long long lParam)
  {
    if (msg & Window::WND_MSG_MOUSE_KEY_MASK)
    {
      unsigned int key = 0;
      switch (wParam)
      {
        case Window::KEY_LBMOUSE: key = BKEY_LBMOUSE; break;
        case Window::KEY_RBMOUSE: key = BKEY_RBMOUSE; break;
        case Window::KEY_MBMOUSE: key = BKEY_MBMOUSE; break;
        default: return;
      }
      const bool pressed = !!(msg & Window::WND_MSG_BMOUSE_DOWN);
      const bool stateChanged = m_Ms.setButtonState(key, pressed);
      if (m_Listener && stateChanged)
        m_Listener->onMouseButtonStateChanged(m_Ms, key, pressed);
    }
    else if (msg & Window::WND_MSG_MOUSE_MOVE)
    {
      const int x = GET_X_LPARAM(lParam);
      const int y = GET_Y_LPARAM(lParam);
      POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

      const int2 oldPos = m_Ms.getPointerPos();
      const int2 newPos = int2{point.x, point.y};

      const bool stateChanged = m_Ms.setPointerPos(newPos);
      if (m_Listener && stateChanged)
        m_Listener->onMouseMove(m_Ms, newPos, newPos - oldPos);
    }
    else if (msg & Window::WND_MSG_MOUSEHWHEEL)
    {
      const int delta = (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
      if (m_Listener)
        m_Listener->onMouseWheelScroll(m_Ms, delta);
    }
    
  }
}
