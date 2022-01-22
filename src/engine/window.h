#pragma once

#include <engine/types.h>

namespace Engine::Window
{
  void InitWindow();

  void PollWndMessages();

  void* GetHwnd();

  Int2 GetWindowSize();
}
