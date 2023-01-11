#include "gui.h"
#include "element_tree_builder.h"
#include "engine/window.h"
#include "scene_tree_builder.h"
#include "engine/qjs/value.hpp"

#include <engine/log.h>
#include <engine/utils/fs.h>
#include <engine/window.h>

namespace Engine::gui
{
  Gui manager;

  void Gui::loadRootUiScript()
  {
    const char* srcFile = "scripts/ui/main.js";
    loginfo("ui: loading root ui from {}", srcFile);

    const string src = Utils::read_file_as_string(srcFile);
    m_Vm.pushCurrentFileName(srcFile);
    m_Vm.eval(src);
  }

  void Gui::init()
  {
    loginfo("gui initialization");
    m_Vm.init();
    loadRootUiScript();

    qjs::Value rootUI = m_Vm.getGlobalObjectProperty("rootUI");
    if (rootUI.isUndefined())
    {
      logerror("ui: globalThis.rootUI is not defined");
      return;
    }
    if (!rootUI.isObject())
    {
      logerror("ui: globalThis.rootUI is not object");
      return;
    }

    ElementTreeBuilder jsParser;
    m_SceneRoot = jsParser.buildFromRootUi(rootUI, m_Vm.getGlobal());

    if (m_SceneRoot.has_value())
    {
      SceneTreeBuilder scenePlacer{Window::get_window_size()};
      scenePlacer.build(m_SceneRoot.value());
    }
  }

  void Gui::tick()
  {
    m_Timers.tick();
  }
}
