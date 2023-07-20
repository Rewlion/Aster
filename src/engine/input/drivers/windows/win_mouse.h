#pragma once

#include <engine/input/drivers/buttons.h>
#include <engine/input/drivers/pointer.h>
#include <engine/window.h>
#include <engine/utils/bit_capacity.hpp>

namespace Engine::Input
{
  class WinMouse: public IPointer
  {
    public:
      auto setButtonState(const int key, const bool pressed) -> bool override;
      auto isButtonPressed(const int key) const -> bool override;
      auto setPointerPos(const int2 pos) -> bool override;
      auto getPointerPos() const -> int2 override { return m_PointerPos; }

    private:
      Utils::BitCapacity<BKEY_MOUSE_COUNT> m_Buttons;
      int2 m_PointerPos;
  };

  class WinMouseDriver: public IPointerDriver, Window::IWndProcHandler
  {
    public:
      WinMouseDriver();
      virtual
      ~WinMouseDriver();

      virtual
      void handleWndEvent(void* hwnd,
                          unsigned int msg,
                          unsigned long long wParam,
                          long long lParam) override;

      virtual
      void setListener(IPointerListener* listener) override { m_Listener = listener; }

    private:
      IPointerListener* m_Listener;
      WinMouse m_Ms;
  };
}