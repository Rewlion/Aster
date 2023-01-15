#pragma once

#include "element.h"
#include "runtime_state.h"

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
      bool rebuildElem(Element& parent, Element& child, const DirtyStates& dirty_states);

      inline const std::optional<Element>& getRoot() const { return m_Root; }

      void setMouseCursorPos(const float2 pos);
      void setMouseClickState(const bool clicked);

    private:
      bool setHoveredElem(Element* parent, const float2 pos);

    private:
      RuntimeState& m_RtState;
      BehaviorsStorage& m_Behaviors;
      std::optional<Element> m_Root;

      Element* m_HoveredElem;
      float2 m_MousePos;
  };
}
