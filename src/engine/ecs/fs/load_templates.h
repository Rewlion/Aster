#pragma once

#include <engine/types.h>

namespace Engine::ECS
{
  class Registry;

  void AddTemplatesFromBlk(Registry& manager, const string& blk);
}
