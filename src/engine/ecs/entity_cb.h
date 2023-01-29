#pragma once

#include <EASTL/functional.h>

class EntityId;

namespace Engine::ECS
{
  class EntityInitializer;
  typedef eastl::function<void(const EntityId&, EntityInitializer&)> CreationCb;
}
