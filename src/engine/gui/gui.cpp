#include "gui.h"

#include <engine/log.h>
#include <engine/qjs/value.hpp>
#include <engine/utils/fs.h>
#include <engine/window.h>

namespace Engine::gui
{
  Gui manager;

  Gui::Gui()
    : m_Scene(m_RtState, m_Behaviors)
  {
  }

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
    m_Vm.setUserState(&m_RtState);
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

    m_Behaviors.init();
    m_Scene.setScreenSize(Engine::Window::get_window_size());
    m_Scene.initFromJS(rootUI);
  }

  void Gui::tick()
  {
    if (m_RtState.reactStorage.hasDirtyStates())
      m_Scene.rebuildDirtyElems(m_RtState.reactStorage.getDirtyStates());

    m_RtState.timers.tick();
  }

  void Gui::setMouseCursorPos(const int2 pos)
  {
    m_Scene.setMouseCursorPos(pos);
  }

  void Gui::setMouseClickState(const bool clicked)
  {
    m_Scene.setMouseClickState(clicked);
  }
}
