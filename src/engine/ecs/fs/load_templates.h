#pragma once

#include <engine/types.h>

namespace Engine::ECS
{
  class Registry;

  void add_templates_from_blk(Registry& manager, const string& blk);
}
