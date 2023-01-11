#pragma once

#include "element.h"
#include "react_states_storage.h"
#include "timer.h"

#include <engine/qjs/qjs.h>

#include <optional>

namespace Engine::gui
{
  class ReactStateClass;

  class Gui
  {
    public:
      void init();
      inline const std::optional<Element>& getRootUI() const
      {
        return m_SceneRoot;
      }

      inline TimersPool& getTimersPool()
      {
        return m_Timers;
      }

      void tick();

      inline void addReactState(ReactStateClass* state)
      {
        m_ReactStorage.add(state);
      }

      inline void removeReactState(ReactStateClass* state)
      {
        m_ReactStorage.remove(state);
      }

    private:
      void loadRootUiScript();

    private:
      qjs::VM m_Vm;
      TimersPool m_Timers;
      std::optional<Element> m_SceneRoot;
      ReactStatesStorage m_ReactStorage;
  };

  extern Gui manager;
}
