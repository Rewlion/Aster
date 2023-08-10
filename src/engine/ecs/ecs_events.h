#pragma once

#include "events.h"

namespace ecs
{
  struct OnEntityCreated : public Event
  {
    EntityId eid;
    
    EVENT_CONSTRUCTOR(OnEntityCreated)
    {}
  };

  struct OnGameTick : public Event
  {
    EVENT_CONSTRUCTOR(OnGameTick)
    {}
  };
}

