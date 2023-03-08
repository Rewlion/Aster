#pragma once

#include <engine/types.h>

namespace Engine::Window
{
  class IWndProcHandler
  {
    public:
      virtual
      void handleWndEvent(void* hwnd,
                          unsigned int msg,
                          unsigned long long wParam,
                          long long param) = 0;
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
