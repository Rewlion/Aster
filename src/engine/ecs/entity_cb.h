#pragma once

#include <EASTL/functional.h>

class EntityId;

namespace Engine::ECS
{
  class EntityComponents;
  typedef eastl::function<void(const EntityId&, EntityComponents&)> CreationCb;
}
