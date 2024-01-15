#pragma once

#include <engine/data/ed.h>

namespace Engine
{
  void load_app_settings(const string_view path);
  auto get_app_settings() -> ed::Scope&;
}
