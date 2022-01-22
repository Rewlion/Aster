#pragma once

#include <engine/types.h>

namespace Engine::ECS
{
  class Registry;

  void AddTemplatesFromBlk(Registry& ecs, const string& blk);
}
