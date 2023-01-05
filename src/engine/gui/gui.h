#pragma once

#include "element.h"

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

    private:
      void loadRootUiScript();

    private:
      qjs::VM m_Vm;
      std::optional<Element> m_SceneRoot;
  };

  extern Gui manager;
}
