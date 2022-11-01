#include "settings.h"

#include <engine/assert.h>
#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <engine/log.h>

namespace Engine
{
  static DataBlock SETTINGS_BLK;

  void load_app_settings(const char* blkPath)
  {
    ASSERT_FMT(load_blk_from_file(&SETTINGS_BLK, blkPath),
      "failed to load application settings from {}", blkPath);
  }

  DataBlock* get_app_settings()
  {
    return &SETTINGS_BLK;
  }
}