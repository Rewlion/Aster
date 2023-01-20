#include "scene.h"
#include "behavior.h"
#include "element_tree_builder.h"
#include "scene_placer.h"

#include <engine/window.h>

namespace Engine::gui
{
  Scene::Scene(RuntimeState& state, BehaviorsStorage& behaviors)
    : m_RtState(state)
    , m_Behaviors(behaviors)
    , m_HoveredElem(nullptr)
  {
  }

  void Scene::initFromJS(const qjs::Value& root_ui)
  {
    ElementTreeBuilder jsParser{m_Behaviors};
    m_Root = jsParser.buildFromRootUi(root_ui);

    if (m_Root.has_value())
      ScenePlacer{Window::get_window_size()}.placeRoot(m_Root.value());
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
      ElementTreeBuilder rebuilder{m_Behaviors};
      std::optional<Element> newElem{std::in_place};
      newElem = rebuilder.buildDynamicElem(child.constructor);
      if (newElem.has_value())
      {
        child = std::move(newElem.value());
        ScenePlacer{m_ScreenSize}.placeChilds(parent);
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

  void Scene::setMouseCursorPos(const float2 pos)
  {
    if (m_Root.has_value())
    {
      if (!setHoveredElem(&m_Root.value(), pos) && m_HoveredElem)
      {
        m_Behaviors.getBehavior(BehaviorType::Button)
          ->onStateChange(*m_HoveredElem, BhvStateChange::OnMouseHoverEnd, BHV_RES_NONE);
        m_HoveredElem = nullptr;
      }
    }
  }

  bool Scene::setHoveredElem(Element* parent, const float2 pos)
  {
    const auto getBtnBhv = [](Element* elem) {
      for (const auto& bhv: elem->params.behaviors)
        if (bhv->getType() == BehaviorType::Button)
          return bhv;
      
      return (IBehavior*)nullptr;
    };

    if (parent->isInside(pos))
    {
      if (IBehavior* parentBtn = getBtnBhv(parent))
      {
        bool isSame = m_HoveredElem == parent;
        if (!isSame && m_HoveredElem)
        {
          IBehavior* prevBtn = getBtnBhv(m_HoveredElem);
          prevBtn->onStateChange(*m_HoveredElem, BhvStateChange::OnMouseHoverEnd, BHV_RES_NONE);
        }
        parentBtn->onStateChange(*parent, BhvStateChange::OnMouseHoverBegin, BHV_RES_NONE);
        m_HoveredElem = parent;
        m_MousePos = pos;
        return true;
      }
      else
      {
        for (auto& child: parent->childs)
          if (setHoveredElem(&child, pos))
            return true;
      }
    }

    return false;
  }

  void Scene::setMouseClickState(const bool clicked)
  {
    if (!m_HoveredElem)
      return;

    Element* elem = m_HoveredElem;

    const auto state = clicked ?
      BhvStateChange::OnMouseClickBegin :
      BhvStateChange::OnMouseClickEnd;

    const auto findNextChildToProcess = [](Element* elem, float2 pos)
    {
      for (auto& child: elem->childs)
      {
        if (child.isInside(pos))
          return &child;
      }

      return (Element*)nullptr;
    };

    while (elem)
    {
      BhvResult res = BHV_RES_NONE;
      for (IBehavior* bhv: elem->params.behaviors)
        res = (BhvResult) (res | bhv->onStateChange(*elem, state, res));

      if (res & BHV_RES_PROCESSED)
        break;

      elem = findNextChildToProcess(elem, m_MousePos);
    }
  }

  void Scene::setScreenSize(const float2 size)
  {
    m_ScreenSize = size;
  }
}
