#pragma once

#include <engine/qjs/forward_decl.h>

namespace Engine::gui
{
  class ReactStateClass;

  class ReactStateRegistration
  {
    public:
      ReactStateRegistration() = default;
      ReactStateRegistration(const qjs::Value&);
      ReactStateRegistration(ReactStateRegistration&&);
      ~ReactStateRegistration();

      inline const ReactStateClass* getState() const
      {
        return m_State;
      }

    private:
      ReactStateClass* m_State = nullptr;
      JSContext* m_Ctx = nullptr;
  };
}
