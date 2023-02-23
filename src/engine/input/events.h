#include "actions.h"

#include <engine/types.h>
#include <engine/ecs/events.h>

struct ButtonActionInputEvent: public ecs::Event
{
  string_hash action = -1;
  Engine::Input::ButtonStatus status = Engine::Input::ButtonStatus::Release;

  EVENT_CONSTRUCTOR(ButtonActionInputEvent)
  {
  }
};
