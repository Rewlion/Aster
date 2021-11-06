#include <iostream>

#include <engine/engine.h>
#include <engine/log.h>

int main()
{
  ::load_app_settings("game_data/settings.blk");
  ::init_log();
}