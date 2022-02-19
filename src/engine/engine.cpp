#include "engine.h"

#include <engine/ecs/archetype.h>
#include <engine/ecs/components.h>
#include <engine/ecs/components_accessor.h>
#include <engine/ecs/registry.h>
#include <engine/input/input.h>
#include <engine/level.h>
#include <engine/render/gapi/gapi.h>
#include <engine/render/world_render.h>
#include <engine/settings.h>
#include <engine/time.h>
#include <engine/types.h>
#include <engine/window.h>

namespace Engine
{
  void Init()
  {
    Time::InitClock();

    LoadAppSettings("game_data/settings.blk");
    InitLog();
    Window::InitWindow();

    gapi::init();
    Input::manager.Init();

    ECS::InitEcsFromSettings();
    LoadLevel( Engine::GetAppSettings()->GetText("init_level") );

    Render::world_render.Init();
  }

  void StartTick()
  {
    for(;;)
    {
      Time::Tick();

      Window::PollWndMessages();
      Input::manager.ProcessInput();

      ECS::manager.tick();

      Render::world_render.Render();
    }
  }
}