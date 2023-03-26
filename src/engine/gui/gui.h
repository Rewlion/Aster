#pragma once

#include "behavior.h"
#include "scene.h"

#include <engine/input/input_router.h>
#include <engine/qjs/qjs.h>

#include <optional>
#include <limits.h>

namespace Engine::gui
{
  class ReactStateClass;

  class Gui : public Input::IInputRouterListener
  {
    public:
      Gui();
      ~Gui();

      static
      void init();
      static
      void destroy();

      int getInputRouterPriority() override { return PRIORITY_DONT_CARE; };

      auto onMouseButtonStateChanged(const Input::IPointer& device,
                                     const int key, 
                                     const bool pressed) -> Input::InputRouterProcessStatus override;

      auto onMouseMove(const Input::IPointer& device,
                       const int2 new_pos,
                       const int2 delta) -> Input::InputRouterProcessStatus override;

      static
      void tick();

      static
      auto getElemsToRender() -> const eastl::vector<Element*>& { return m_This->m_Scene.getElemsToRender(); }

    private:
      void setMouseCursorPos(const int2 pos);
      void setMouseClickState(const bool clicked);

      void loadRootUiScript();

    private:
      static Gui* m_This;

      qjs::VM m_Vm;
      RuntimeState m_RtState;
      BehaviorsStorage m_Behaviors;
      Scene m_Scene{m_RtState, m_Behaviors};
  };
}
