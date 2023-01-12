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
      void rebuildDirtyElems();
      bool rebuildElem(Element& parent, Element& child, const eastl::vector_set<const ReactStateClass*>& dirty_states);

      inline void addReactState(ReactStateClass* state)
      {
        m_ReactStorage.add(state);
      }

      inline void removeReactState(ReactStateClass* state)
      {
        m_ReactStorage.remove(state);
      }

      inline void useReactStateInUi(const ReactStateClass* state)
      {
        m_ReactStorage.useUiState(state);
      }

      inline void removeReactStateFromUi(const ReactStateClass* state)
      {
        m_ReactStorage.removeUiState(state);
      }

      inline void markDirtyState(const ReactStateClass* state)
      {
        m_ReactStorage.markDirtyState(state);
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
