#pragma once

#include "registry.h"

namespace Engine::ECS
{
  Registry& get_registry();
  void init_from_settings();
}
