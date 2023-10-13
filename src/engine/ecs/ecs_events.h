#pragma once

#include <engine/ecs/events.h>

namespace ecs
{
  ECS_EVENT_BEGIN(OnEntityCreated)
    ECS_EVENT_FIELD(ecs::EntityId eid)
  ECS_EVENT_END()
}
