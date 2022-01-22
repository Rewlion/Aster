#include "settings.h"

#include <engine/datablock/datablock.h>
#include <engine/datablock/utils.h>
#include <assert.h>

static DataBlock SETTINGS_BLK;

void LoadAppSettings(const char* blkPath)
{
  if (!LoadBlkFromFile(&SETTINGS_BLK, blkPath))
    assert("failed to load blk");
}

DataBlock* GetAppSettings()
{
  return &SETTINGS_BLK;
}
