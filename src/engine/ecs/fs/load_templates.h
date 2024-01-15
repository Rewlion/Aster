#pragma once

#include <engine/types.h>

namespace ecs
{
  class Registry;

  void add_templates_from_ed(Registry& manager, const string_view file);
}
