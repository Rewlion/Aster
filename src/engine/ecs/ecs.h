#pragma once

#include "registry.h"

namespace ecs
{
  auto get_registry() -> Registry&;
  void init_from_settings();
}
