#pragma once

#include <engine/types.h>

namespace Engine::Input
{
  class IPointer
  {
    virtual
    auto setButtonState(const int key, const bool pressed) -> bool = 0;

    virtual
    auto isButtonPressed(const int key) const -> bool = 0;

    virtual
    auto setPointerPos(const int2 pos) -> bool = 0;

    virtual
    auto getPointerPos() const -> int2 = 0;
  };

  class IPointerListener
  {
    public:
      virtual
      void onMouseButtonStateChanged(const IPointer& device, const int key, const bool pressed) = 0;

      virtual
      void onMouseMove(const IPointer& device, const int2 new_pos, const int2 delta) = 0;
  };

  class IPointerDriver
  {
    public:
      virtual
      ~IPointerDriver() {};

      virtual
      void setListener(IPointerListener*) = 0;
  };

  auto create_pointer_driver() -> IPointerDriver*;
}
