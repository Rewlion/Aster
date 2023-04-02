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
}

