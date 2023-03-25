#pragma once

#include "element.h"
#include "behavior.h"
#include "runtime_state.h"

#include <EASTL/vector.h>

#include <optional>

namespace Engine::gui
{
  class RuntimeState;
  class BehaviorsStorage;

  class Scene
  {
    public:
      Scene(RuntimeState&, BehaviorsStorage&);
      void initFromJS(const qjs::Value& root_ui);

      void rebuildDirtyElems(const DirtyStates& dirty_states);
      bool rebuildElem(Element& parent, Element::Ptr& child, const DirtyStates& dirty_states);

      inline const eastl::vector<Element*>& getElemsToRender() const { return m_RenderElems; }

      void setMouseCursorPos(const float2 pos);
      void setMouseClickState(const bool clicked);

      void setScreenSize(const float2);

    private:
      void processMouseEvent(const BhvStateChange);
      void rebuildStackedElems();
      void rebuildStackedElemsInternal();
      void sortStackedElemsByZOrder();

    private:
      RuntimeState& m_RtState;
      BehaviorsStorage& m_Behaviors;
      Element::Ptr m_Root;
      eastl::vector<Element*> m_ElemsWithCursor;
      eastl::vector<Element*> m_InputElems;
      eastl::vector<Element*> m_RenderElems;
      float2 m_ScreenSize;

      float2 m_CursorPos;
      bool m_IsInsideCursorArea;
  };
}
