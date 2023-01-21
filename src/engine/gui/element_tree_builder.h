#pragma once

#include "element.h"
#include "constants.h"

#include <engine/qjs/forward_decl.h>

namespace Engine::gui
{
  class BehaviorsStorage;

  class ElementTreeBuilder
  {
    public:
      ElementTreeBuilder(BehaviorsStorage&);
      Element::Ptr buildFromRootUi(const qjs::Value& root_ui);
      Element::Ptr buildDynamicElem(const qjs::Value&, const size_t z_order);
    private:
      Element::Ptr buildStaticElem(const qjs::Value&, const size_t z_order);
      Element::Ptr buildElem(const qjs::Value&, const size_t z_order);
      Element::Params buildParams(const qjs::Value&) const;
      Element::DynamicParams buildDynamicParams(const qjs::Value& static_elem, const qjs::Value& constructor) const;
      eastl::vector<Element::Ptr> buildChilds(const qjs::Value&, const size_t z_order);
      
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
