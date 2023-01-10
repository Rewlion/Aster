#pragma once

#include "element.h"

namespace Engine::gui
{
  class SceneTreeBuilder
  {
    public:
      SceneTreeBuilder(const float2 render_size)
        : m_RenderSize(render_size)
      {
      }

      void build(Element& tree);

    private:
      void placeChilds(Element& parent) const;
      void placeChildsInRow(Element& parent) const;
      void placeChildsInFlexRow(Element& parent) const;
      void placeChildsInColumn(Element& parent) const;
      void placeChildsInFlexColumn(Element& parent) const;
      void absPlaceChild(const float2 parent_pos, const float2 parent_size, Element& child) const;
      float2 getAnchorPos(const float2 parent_pos, const float2 parent_size, const Element& child) const;
      float getRelativeHalignPos(const float parent_width, const Element& child) const;
      float getRelativeValignPos(const float parent_height, const Element& child) const;
      float2 getRelativePos(const Element& child) const;

    private:
      float2 m_RenderSize;
  }; 
}