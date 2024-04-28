#pragma once

#include <engine/ecs/ecs_events.h>

namespace Engine
{
  ECS_EVENT_BEGIN(OnGameTick)
  ECS_EVENT_END()

  ECS_EVENT_BEGIN(SunOrientationChanged)
    ECS_EVENT_FIELD(float newAzimuth)
    ECS_EVENT_FIELD(float newAltitude)
  ECS_EVENT_END()
}
