#pragma once

#include "keyboard.h"
#include "pointer.h"

namespace Engine::Input
{
  void init_drivers();
  void destroy_drivers();

  IKeyboardDriver* get_keyboard_driver();
  IPointerDriver* get_pointer_driver();
}
