#include "engine.h"

#include <engine/assets/assets_manager.h>
#include <engine/console/console.h>
#include <engine/debug_marks.h>
#include <engine/ecs/ecs.h>
#include <engine/editor/editor.h>
#include <engine/events.h>
#include <engine/fs/file_watch.h>
#include <engine/fs/vfs.h>
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

namespace Engine
{
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
    vfs::init();

    Window::init_window();

    gapi::init();
    assets_manager.init();
    
    init_input();
    editor::Manager::init();

    const DataBlock* settings = Engine::get_app_settings();
    const string matBinPath = settings->getChildBlock("graphics")->getText("materials_bin");
    tfx::load_materials_bin(matBinPath);

    ecs::init_from_settings();

    load_level(settings->getText("init_level") );

    imgui::Manager::init();
    Render::world_render.init();
    gui::Gui::init();

    console::init();
    
    fs::FilesWatcher::init();
    fs::FilesWatcher::watchFileWrites(matBinPath, tfx::reload_materials_bin);
  }

  void start_tick()
  {
    while(!Window::has_pending_exit())
    {
      PROFILE_CPU_NAMED("Tick");

      fs::FilesWatcher::readNotifies();
      Time::tick();

      Window::poll_wnd_messages();
      Input::Manager::processInput();

      auto& ecsRegistry = ecs::get_registry();
      ecsRegistry.tick();
      ecsRegistry.broadcastEventSync(OnGameTick{});

      gui::Gui::tick();

      imgui::Manager::tick();
      Render::world_render.render();
      
      FrameMarkNamed("MainThread");
    }
  }

  void shutdown()
  {
    ecs::shutdown();
    fs::FilesWatcher::shutdown();
    gapi::shutdown();
  }
}
