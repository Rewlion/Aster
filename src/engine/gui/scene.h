#pragma once

#include "element.h"
#include "runtime_state.h"

#include <optional>

namespace Engine::gui
{
  class RuntimeState;

  class Scene
  {
    public:
      Scene(RuntimeState&);
      void initFromJS(const qjs::Value& root_ui, const qjs::Value& global);

      void rebuildDirtyElems(const DirtyStates& dirty_states);
      bool rebuildElem(Element& parent, Element& child, const DirtyStates& dirty_states);

      inline const std::optional<Element>& getRoot() const { return m_Root; }

    private:
      RuntimeState& m_RtState;
      std::optional<Element> m_Root;
  };
}
