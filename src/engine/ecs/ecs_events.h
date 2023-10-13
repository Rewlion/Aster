#pragma once

#include "events.h"

namespace ecs
{
  ECS_EVENT_BEGIN(OnEntityCreated)
    ECS_EVENT_FIELD(ecs::EntityId eid)
  ECS_EVENT_END()

  ECS_EVENT_BEGIN(OnGameTick)
  ECS_EVENT_END()
}
