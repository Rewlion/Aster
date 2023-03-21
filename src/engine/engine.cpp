#include "engine.h"

#include <engine/assets/assets_manager.h>
#include <engine/console/console.h>
#include <engine/ecs/ecs.h>
#include <engine/gapi/gapi.h>
#include <engine/gui/gui.h>
#include <engine/input/drivers/drivers.h>
#include <engine/input/input.h>
#include <engine/input/input_router.h>
#include <engine/level.h>
#include <engine/render/imgui.h>
#include <engine/render/world_render.h>
#include <engine/settings.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>
#include <engine/types.h>
#include <engine/window.h>
#include <engine/work_cycle/camera.h>

namespace Engine
{
  void register_engine_events()
  {
  }

  void init_input()
  {
    Input::init_drivers();
    Input::InputRouter::init();
    Input::InputManager::init();
  }

  void init()
  {
    InitLog();
    Time::init_clock();

    load_app_settings("game_data/settings.blk");
    Window::init_window();

    gapi::init();
    assets_manager.init();
    
    init_input();

    const DataBlock* settings = Engine::get_app_settings();
    tfx::load_materials_bin(settings->getChildBlock("graphics")->getText("materials_bin"));

    ecs::init_from_settings();
    register_engine_events();

    load_level(settings->getText("init_level") );

    Render::ImGuiManager::init();
    Render::world_render.init();
    gui::manager.init();

    console::init();
  }

  void start_tick()
  {
    for(;;)
    {
      Time::tick();

      Window::poll_wnd_messages();
      Input::InputManager::processInput();

      ecs::get_registry().tick();
      gui::manager.tick();

      const CameraData camera = get_camera();
      Render::ImGuiManager::tick();
      Render::world_render.render(camera);
    }
  }
}
