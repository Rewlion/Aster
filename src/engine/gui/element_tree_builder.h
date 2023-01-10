#pragma once

#include "element.h"
#include "engine/gui/constants.h"

#include <engine/qjs/forward_decl.h>

#include <optional>

namespace Engine::gui
{
  class ElementTreeBuilder
  {
    public:
      std::optional<Element> buildFromRootUi(const qjs::Value& root_ui, const qjs::Value& global);

    private:
      std::optional<Element> buildStaticElem(const qjs::Value&) const;
      std::optional<Element> buildDynamicElem(const qjs::Value&) const;
      std::optional<Element> buildElem(const qjs::Value&) const;
      std::optional<Element::Params> buildParams(const qjs::Value&) const;
      eastl::vector<Element> buildChilds(const qjs::Value&) const;
      
      SizeParam getSize(qjs::ObjectView&) const;
      int2 getPos(qjs::ObjectView&) const;
      ColorParam getColor(qjs::ObjectView&) const;
    private:
      const qjs::Value* m_Global;
  };
}
