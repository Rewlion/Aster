#pragma once

#include <engine/types.h>

namespace Engine::Window
{
  void init_window();

  void poll_wnd_messages();

  void* get_hwnd();

  int2 get_window_size();
}
