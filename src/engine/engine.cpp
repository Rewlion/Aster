#include "engine.h"

#include <engine/assets/assets_manager.h>
#include <engine/console/console.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/ecs_events.h>
#include <engine/editor/editor.h>
#include <engine/gapi/gapi.h>
#include <engine/gui/gui.h>
#include <engine/input/drivers/drivers.h>
#include <engine/input/input.h>
#include <engine/input/input_router.h>
#include <engine/level.h>
#include <engine/render/imgui/imgui.h>
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
    ecs::get_registry().registerEvent<ecs::OnEntityCreated>();
  }

  void init_input()
  {
    Input::init_drivers();
    Input::Router::init();
    Input::Manager::init();
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
    editor::Manager::init();

    const DataBlock* settings = Engine::get_app_settings();
    tfx::load_materials_bin(settings->getChildBlock("graphics")->getText("materials_bin"));

    ecs::init_from_settings();
    register_engine_events();

    load_level(settings->getText("init_level") );

    imgui::Manager::init();
    Render::world_render.init();
    gui::Gui::init();

    console::init();
  }

  void start_tick()
  {
    for(;;)
    {
      Time::tick();

      Window::poll_wnd_messages();
      Input::Manager::processInput();

      ecs::get_registry().tick();
      gui::Gui::tick();

      const CameraData camera = get_camera();
      imgui::Manager::tick();
      Render::world_render.render(camera);
    }
  }
}
