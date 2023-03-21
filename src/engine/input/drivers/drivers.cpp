#include "drivers.h"

#include <engine/assert.h>

namespace Engine::Input
{
  IKeyboardDriver* keyboard_driver = nullptr;
  IPointerDriver* pointer_driver = nullptr;

  void init_drivers()
  {
    ASSERT(keyboard_driver == nullptr);
    keyboard_driver = create_keyboard_driver();
    pointer_driver = create_pointer_driver();
  }

  void destroy_drivers()
  {
    ASSERT(keyboard_driver);
    delete keyboard_driver;
    keyboard_driver = nullptr;

    ASSERT(pointer_driver);
    delete pointer_driver;
    pointer_driver = nullptr;
  }

  IKeyboardDriver* get_keyboard_driver()
  {
    return keyboard_driver;
  }

  IPointerDriver* get_pointer_driver()
  {
    return pointer_driver;
  }
}