#include "scene.h"
#include "element_tree_builder.h"
#include "scene_placer.h"

#include <engine/window.h>

namespace Engine::gui
{
  Scene::Scene(RuntimeState& state)
    : m_RtState(state)
  {
  }

  void Scene::initFromJS(const qjs::Value& root_ui, const qjs::Value& global)
  {
    ElementTreeBuilder jsParser;
    m_Root = jsParser.buildFromRootUi(root_ui, global);

    if (m_Root.has_value())
      ScenePlacer{}.placeRoot(m_Root.value(), Window::get_window_size());
  }

  void Scene::rebuildDirtyElems(const DirtyStates& dirty_states)
  {
    if (!m_Root.has_value())
      return;

    Element fakeParent;
    fakeParent.sceneParams = Element::SceneParams{
      .pos = {0, 0},
      .size = Window::get_window_size()
    };

    rebuildElem(fakeParent, m_Root.value(), dirty_states);
  }

  bool Scene::rebuildElem(Element& parent, Element& child, const DirtyStates& dirty_states)
  {
    const auto isDirty = [&dirty_states](const Element& elem) {
      if (!elem.isDynamic())
        return false;
    
      for (const auto* state : dirty_states)
        for (const auto& reg: elem.observes)
          if (reg.getState() == state)
            return true;

      return false;
    };
  
    if (isDirty(child))
    {
      ElementTreeBuilder rebuilder;
      std::optional<Element> newElem = rebuilder.buildDynamicElem(child.constructor);
      if (newElem.has_value())
      {
        child = std::move(newElem.value());
        ScenePlacer{}.placeChilds(parent);
        return true;
      }
      else
        return false;
    }

    eastl::vector<size_t> idsToRemove;
    for (Element& e: child.childs)
    {
      if (!rebuildElem(child, e, dirty_states))
        idsToRemove.emplace_back(child.id);
    }

    if (!idsToRemove.empty())
    {
      eastl::erase_if(child.childs, [&idsToRemove](const Element& el) {
        return eastl::find(idsToRemove.begin(), idsToRemove.end(), el.id) != idsToRemove.end();
      });
    }

    return true;
  }

}
