#include "actions.h"

#include <engine/types.h>
#include <engine/ecs/events.h>

struct ButtonActionInputEvent: public Event
{
  string_hash action = -1;
  ButtonStatus status = ButtonStatus::Release;

  EVENT_CONSTRUCTOR(ButtonActionInputEvent)
  {
  }
};
