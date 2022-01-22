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
  InitEngineClock();

  LoadAppSettings("game_data/settings.blk");
  InitLog();
  InitWindow();

  InitEcsFromSettings();
  LoadLevel( ::GetAppSettings()->GetText("init_level") );

  input_manager.Init();
  input_manager.SetActionset(str_hash("FlyingCamera"));

  for(;;)
  {
    TickTime();

    PollWndMessages();
    input_manager.ProcessInput();

    ecs.tick();
  }
}
