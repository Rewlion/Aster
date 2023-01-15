#pragma once

#include "behavior.h"
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

      void setMouseCursorPos(const int2 pos);
      void setMouseClickState(const bool clicked);
    private:
      void loadRootUiScript();

    private:
      qjs::VM m_Vm;
      RuntimeState m_RtState;
      BehaviorsStorage m_Behaviors;
      Scene m_Scene;
  };

  extern Gui manager;
}
