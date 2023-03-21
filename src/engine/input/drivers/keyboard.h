#pragma once

namespace Engine::Input
{
  class IKeyboard;
  class IKeyboardListener
  {
    public:
      virtual
      void onKeyStateChanged(const IKeyboard& device, const int key, const bool pressed) = 0;
  };

  class IKeyboard
  {
    public:
      virtual
      auto setButtonState(const int key, const bool pressed) -> bool = 0;

      virtual
      auto isButtonPressed(const int key) const -> bool = 0;

      virtual
      ~IKeyboard() {}
  };

  class IKeyboardDriver
  {
    public:
      virtual
      ~IKeyboardDriver() {}

      virtual
      void setListener(IKeyboardListener*) = 0;
  };

  auto create_keyboard_driver() -> IKeyboardDriver*;
};
