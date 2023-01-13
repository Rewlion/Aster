#include "react_state_registration.h"
#include "gui.h"
#include "react_state.h"
#include "runtime_state.h"

#include <engine/qjs/value.hpp>

namespace Engine::gui
{
  ReactStateRegistration::ReactStateRegistration(const qjs::Value& v)
  {
    m_State = ReactStateClass::unpack(v);
    m_Ctx = v.getContext();

    qjs::get_user_state<RuntimeState>(v.getContext())
      ->reactStorage.incUiStateRC(m_State);
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
      qjs::get_user_state<RuntimeState>(m_Ctx)
        ->reactStorage.decUiStateRC(m_State);

      m_State = nullptr;
      m_Ctx = nullptr;
    }
  }
}
