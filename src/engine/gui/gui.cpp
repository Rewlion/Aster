#include "gui.h"

#include <engine/assert.h>
#include <engine/input/drivers/buttons.h>
#include <engine/log.h>
#include <engine/qjs/value.hpp>
#include <engine/utils/fs.h>
#include <engine/window.h>

namespace Engine::gui
{
  Gui* Gui::m_This = nullptr;

  Gui::Gui()
  {
    Input::Router::registerListener(this);

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

  Gui::~Gui(){
    Input::Router::unregisterListener(this);
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
    ASSERT(m_This == nullptr);
    loginfo("gui initialization");
    m_This = new Gui;
  }

  void Gui::destroy()
  {
    ASSERT(m_This != nullptr);
    delete m_This;
    m_This = nullptr;
  }

  auto Gui::onMouseButtonStateChanged(const Input::IPointer& device,
                                      const int key, 
                                      const bool pressed) -> Input::InputRouterProcessStatus
  {
    if (key == Input::BKEY_LBMOUSE)
      setMouseClickState(pressed);

    return Input::InputRouterProcessStatus::ProcessFurther;
  }

  auto Gui::onMouseMove(const Input::IPointer& device,
                        const int2 new_pos,
                        const int2 delta) -> Input::InputRouterProcessStatus
  {
    setMouseCursorPos(new_pos);
    return Input::InputRouterProcessStatus::ProcessFurther;
  }

  void Gui::tick()
  {
    if (m_This->m_RtState.reactStorage.hasDirtyStates())
    {
      m_This->m_Scene.rebuildDirtyElems(m_This->m_RtState.reactStorage.getDirtyStates());
      m_This->m_RtState.reactStorage.clearDirtyStates();
    }

    m_This->m_RtState.timers.tick();
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
