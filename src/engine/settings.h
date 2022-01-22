#pragma once

#include <engine/datablock/datablock.h>

namespace Engine
{
  void LoadAppSettings(const char* path);
  DataBlock* GetAppSettings();
}
