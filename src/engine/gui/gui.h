#pragma once

#include "element.h"
#include "timer.h"

#include <engine/qjs/qjs.h>

#include <optional>

namespace Engine::gui
{
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

    private:
      void loadRootUiScript();

    private:
      qjs::VM m_Vm;
      TimersPool m_Timers;
      std::optional<Element> m_SceneRoot;
  };

  extern Gui manager;
}
