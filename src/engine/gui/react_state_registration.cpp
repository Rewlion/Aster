#include "react_state_registration.h"
#include "react_state.h"
#include "gui.h"

#include <engine/qjs/value.hpp>

namespace Engine::gui
{
  ReactStateRegistration::ReactStateRegistration(const qjs::Value& v)
  {
    m_State = ReactStateClass::unpack(v);
    gui::manager.useReactStateInUi(m_State);
  }

  ReactStateRegistration::ReactStateRegistration(ReactStateRegistration&& rvl)
  {
    this->~ReactStateRegistration();
    std::swap(m_State, rvl.m_State);
  }

  ReactStateRegistration::~ReactStateRegistration()
  {
    if (m_State)
    {
      gui::manager.removeReactStateFromUi(m_State);
      m_State = nullptr;
    }
  }
}
