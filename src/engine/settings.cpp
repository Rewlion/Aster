#include "settings.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <assert.h>

namespace Engine
{
  static DataBlock SETTINGS_BLK;

  void load_app_settings(const char* blkPath)
  {
    if (!load_blk_from_file(&SETTINGS_BLK, blkPath))
      assert("failed to load blk");
  }

  DataBlock* get_app_settings()
  {
    return &SETTINGS_BLK;
  }
}