#pragma once

#include "events.h"

namespace ecs
{
  struct OnEntityCreated : public Event
  {
    EVENT_CONSTRUCTOR(OnEntityCreated)
    {}
  };
}

