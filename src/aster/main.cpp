#include <iostream>

#include <engine/engine.h>

int main()
{
  ::load_app_settings("game_data/settings.blk");
  ::init_log();
  ::init_window();

  for(;;)
  {
    ::poll_input_events();
  }
}
