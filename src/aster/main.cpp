#include "events.h"

#include <engine/engine.h>

#include <engine/input/input.h>
#include <engine/time.h>
#include <engine/level.h>
#include <engine/ecs/registry.h>
#include <engine/ecs/components.h>
#include <engine/ecs/archetype.h>
#include <engine/ecs/components_accessor.h>

int main()
{
  init_engine_clock();

  load_app_settings("game_data/settings.blk");
  init_log();
  init_window();

  init_ecs_from_settings();
  load_level( ::get_app_settings()->get_text("init_level") );

  input_manager.init();
  input_manager.set_actionset(str_hash("FlyingCamera"));

  for(;;)
  {
    tick_time();

    poll_input_events();
    input_manager.process_input();

    ecs.tick();
  }
}
