#pragma once

#include "scene.h"

#include <engine/qjs/qjs.h>

#include <optional>

namespace Engine::gui
{
  class ReactStateClass;

  class Gui
  {
    public:
      Gui();

      void init();
      void tick();

      inline const std::optional<Element>& getRootUI() const { return m_Scene.getRoot(); }
    private:
      void loadRootUiScript();

    private:
      qjs::VM m_Vm;
      RuntimeState m_RtState;
      Scene m_Scene;
  };

  extern Gui manager;
}
