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
      std::optional<Element> buildFromRootUi(const qjs::Value&) const;

    private:
      std::optional<Element> buildElem(const qjs::Value&) const;
      std::optional<Element::Params> buildParams(const qjs::Value&) const;
      eastl::vector<Element> buildChilds(const qjs::Value&) const;
      
      SizeParam getSize(qjs::ObjectView&) const;
      int2 getPos(qjs::ObjectView&) const;
      ColorParam getColor(qjs::ObjectView&) const;
    private:
  };
}
