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
  Engine::Time::InitClock();

  Engine::LoadAppSettings("game_data/settings.blk");
  Engine::InitLog();
  Engine::Window::InitWindow();

  Engine::ECS::InitEcsFromSettings();
  Engine::LoadLevel( Engine::GetAppSettings()->GetText("init_level") );

  Engine::Input::manager.Init();
  Engine::Input::manager.SetActionset(str_hash("FlyingCamera"));

  for(;;)
  {
    Engine::Time::Tick();

    Engine::Window::PollWndMessages();
    Engine::Input::manager.ProcessInput();

    ecs.tick();
  }
}
