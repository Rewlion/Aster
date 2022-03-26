#pragma once

#include <engine/datablock/datablock.h>

namespace Engine
{
  void load_app_settings(const char* path);
  DataBlock* get_app_settings();
}
