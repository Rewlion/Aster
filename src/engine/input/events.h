#include "actions.h"

#include <engine/types.h>
#include <engine/ecs/events.h>

struct ButtonActionInputEvent: public Engine::ECS::Event
{
  string_hash action = -1;
  Engine::Input::ButtonStatus status = Engine::Input::ButtonStatus::Release;

  EVENT_CONSTRUCTOR(ButtonActionInputEvent)
  {
  }
};
