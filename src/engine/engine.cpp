#include "engine.h"

#include <engine/assets/assets_manager.h>
#include <engine/ecs/archetype.h>
#include <engine/ecs/components.h>
#include <engine/ecs/components_accessor.h>
#include <engine/ecs/registry.h>
#include <engine/gapi/gapi.h>
#include <engine/input/input.h>
#include <engine/level.h>
#include <engine/render/world_render.h>
#include <engine/settings.h>
#include <engine/time.h>
#include <engine/types.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

namespace Engine
{
  void init()
  {
    InitLog();
    Time::init_clock();

    load_app_settings("game_data/settings.blk");
    Window::init_window();

    gapi::init();
    assets_manager.init();
    Input::manager.init();

    ECS::init_ecs_from_settings();
    load_level( Engine::get_app_settings()->getText("init_level") );

    Render::world_render.init();
  }

  void start_tick()
  {
    for(;;)
    {
      Time::tick();

      Window::poll_wnd_messages();
      Input::manager.processInput();

      ECS::manager.tick();

      const CameraData camera = get_camera();
      Render::world_render.render(camera);
    }
  }
}