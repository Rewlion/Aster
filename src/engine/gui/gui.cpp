#include "gui.h"
#include "element_tree_builder.h"
#include "engine/window.h"
#include "scene_tree_builder.h"
#include "engine/qjs/value.hpp"

#include <engine/log.h>
#include <engine/utils/fs.h>
#include <engine/window.h>

namespace Engine::gui
{
  Gui manager;

  void Gui::loadRootUiScript()
  {
    const char* srcFile = "scripts/ui/main.js";
    loginfo("ui: loading root ui from {}", srcFile);

    const string src = Utils::read_file_as_string(srcFile);
    m_Vm.pushCurrentFileName(srcFile);
    m_Vm.eval(src);
  }

  void Gui::init()
  {
    loginfo("gui initialization");
    m_Vm.init();
    loadRootUiScript();

    qjs::Value rootUI = m_Vm.getGlobalObjectProperty("rootUI");
    if (rootUI.isUndefined())
    {
      logerror("ui: globalThis.rootUI is not defined");
      return;
    }
    if (!rootUI.isObject())
    {
      logerror("ui: globalThis.rootUI is not object");
      return;
    }

    ElementTreeBuilder jsParser;
    m_SceneRoot = jsParser.buildFromRootUi(rootUI, m_Vm.getGlobal());

    if (m_SceneRoot.has_value())
    {
      SceneTreeBuilder scenePlacer{Window::get_window_size()};
      scenePlacer.build(m_SceneRoot.value());
    }
  }

  void Gui::tick()
  {
    rebuildDirtyElems();
    m_Timers.tick();
  }

  void Gui::rebuildDirtyElems()
  {
    const auto& dirtyStates = m_ReactStorage.getDirtyStates();
    if (dirtyStates.empty() || !m_SceneRoot.has_value())
      return;

    Element fakeParent;
    fakeParent.sceneParams = Element::SceneParams{
      .pos = {0, 0},
      .size = Window::get_window_size()
    };

    rebuildElem(fakeParent, m_SceneRoot.value(), dirtyStates);
  }

  bool Gui::rebuildElem(Element& parent, Element& child, const eastl::vector_set<const ReactStateClass*>& dirty_states)
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
        SceneTreeBuilder placer{Window::get_window_size()};
        placer.placeChilds(parent);
        return true;
      }
      else
        return false;
    }

    eastl::vector_set<size_t> idsToRemove;
    for (Element& e: child.childs)
    {
      if (!rebuildElem(child, e, dirty_states))
        idsToRemove.emplace(child.id);
    }

    eastl::erase_if(child.childs, [&idsToRemove](const Element& el) {
      return idsToRemove.find(el.id) != idsToRemove.end();
    });

    return true;
  }
}
