#pragma once

#include "element.h"
#include "constants.h"

#include <engine/qjs/forward_decl.h>

#include <optional>

namespace Engine::gui
{
  class BehaviorsStorage;

  class ElementTreeBuilder
  {
    public:
      ElementTreeBuilder(BehaviorsStorage&);
      std::optional<Element> buildFromRootUi(const qjs::Value& root_ui);
      std::optional<Element> buildDynamicElem(const qjs::Value&);
    private:
      std::optional<Element> buildStaticElem(const qjs::Value&);
      std::optional<Element> buildElem(const qjs::Value&);
      std::optional<Element::Params> buildParams(const qjs::Value&) const;
      eastl::vector<Element> buildChilds(const qjs::Value&);
      
      eastl::vector<ReactStateRegistration> getObservedReactStates(const qjs::Value&) const;
      SizeParam getSize(qjs::ObjectView&) const;
      int2 getPos(qjs::ObjectView&) const;
      ColorParam getColor(qjs::ObjectView&) const;
      BehaviorsArray getBehaviors(qjs::ObjectView&) const;
    private:
      BehaviorsStorage& m_Behaviors;
  };
}
