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
      std::optional<Element> buildDynamicElem(const qjs::Value&, const size_t z_order);
    private:
      std::optional<Element> buildStaticElem(const qjs::Value&, const size_t z_order);
      std::optional<Element> buildElem(const qjs::Value&, const size_t z_order);
      Element::Params buildParams(const qjs::Value&) const;
      eastl::vector<Element> buildChilds(const qjs::Value&, const size_t z_order);
      
      eastl::vector<ReactStateRegistration> getObservedReactStates(const qjs::Value&) const;
      PointValue getPointValue(const qjs::Value& v) const;
      SizeParam getSize(qjs::ObjectView&) const;
      int2 getPos(qjs::ObjectView&) const;
      ColorParam getColor(qjs::ObjectView&) const;
      BehaviorsArray getBehaviors(qjs::ObjectView&) const;
      qjs::Value getOnClick(qjs::ObjectView&) const;
      PointValue getFontSize(qjs::ObjectView&) const;
    private:
      BehaviorsStorage& m_Behaviors;
  };
}
