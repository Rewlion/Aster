#pragma once

#include <engine/types.h>

namespace Engine::Window
{
  enum
  {
    WND_MSG_KEY_UP      = 1,
    WND_MSG_KEY_DOWN    = 1 << 2,
    WND_MSG_MOUSE_MOVE  = 1 << 3,
    WND_MSG_BMOUSE_UP   = 1 << 4,
    WND_MSG_BMOUSE_DOWN = 1 << 5,

    WND_MSG_KEY_MASK       = WND_MSG_KEY_UP | WND_MSG_KEY_DOWN,
    WND_MSG_MOUSE_KEY_MASK = WND_MSG_BMOUSE_UP | WND_MSG_BMOUSE_DOWN,
  };

  enum
  {
    KEY_LBMOUSE,
    KEY_RBMOUSE,
    KEY_MBMOUSE
  };

  class IWndProcHandler
  {
    public:
      virtual
      void handleWndEvent(void* hwnd,
                          unsigned int msg,
                          unsigned long long wParam,
                          long long lParam) = 0;
  };

  void     reg_wnd_proc_handler(IWndProcHandler*);
  void     unreg_wnd_proc_handler(IWndProcHandler*);

  void     init_window();
  void     poll_wnd_messages();
  void*    get_hwnd();
  int2     get_window_size();
  bool     is_active_window();
  void     show_cursor(const bool);
}
