#include "react_state.h"

#include "gui.h"

namespace Engine::gui
{
  static uint64_t last_registered_state_id = 0;

  ReactStateClass::ReactStateClass(const JSValue obj, JSContext* ctx)
    : m_Id(last_registered_state_id++)
    , m_Obj(obj, ctx)
  {
    gui::manager.addReactState(this);
  }

  ReactStateClass::~ReactStateClass()
  {
    gui::manager.removeReactState(this);
  }
}
