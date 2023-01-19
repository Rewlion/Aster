#include "scene_placer.h"

#include <engine/assert.h>
#include <engine/gui/constants.h>

namespace Engine::gui
{
  ScenePlacer::ScenePlacer(const float2 screen_size)
    : m_ScreenSize(screen_size)
  {
  }

  void ScenePlacer::placeRoot(Element& tree)
  {
    absPlaceChild({0,0}, m_ScreenSize, tree);
  }

  void ScenePlacer::placeChilds(Element& parent) const
  {
    switch (parent.params.flow)
    {
      case Flow::None:
      {
        for (auto& child: parent.childs)
          absPlaceChild(parent.sceneParams.pos, float2{parent.sceneParams.size[0], parent.sceneParams.size[1]}, child);
        break;
      }
      case Flow::Row:
      {
        placeChildsInRow(parent);
        break;
      }
      case Flow::FlexRow:
      {
        placeChildsInFlexRow(parent);
        break;
      }
      case Flow::Column:
      {
        placeChildsInColumn(parent);
        break;
      }
      case Flow::FlexColumn:
      {
        placeChildsInFlexColumn(parent);
        break;
      }
      default: ASSERT(!"unsupported");
    }
  }

  void ScenePlacer::placeChildsInRow(Element& parent) const
  {
    float xOffset = 0.0f;
    for (auto& child: parent.childs)
    {
      child.sceneParams.size = float2{
        child.params.size[0].convert(parent.sceneParams.size, m_ScreenSize),
        child.params.size[1].convert(parent.sceneParams.size, m_ScreenSize)
      };

      child.sceneParams.fontSize = child.params.fontSize.convert(parent.sceneParams.size, m_ScreenSize);

      const float2 parentPos{
        parent.sceneParams.pos.x + xOffset,
        parent.sceneParams.pos.y};

      const float relValign = getRelativeValignPos(parent.sceneParams.size.y, child);

      const float2 childPos = parentPos + float2{0.0f, relValign} + float2{child.params.pos};
      child.sceneParams.pos = childPos;

      xOffset += child.sceneParams.size.x + (float)parent.params.gap;
      placeChilds(child);
    }
  }

   void ScenePlacer::placeChildsInFlexRow(Element& parent) const
  {
    if (parent.childs.empty())
      return;

    float xOffset = 0.0f;
    const size_t childsCount = parent.childs.size();

    const float totalGapSize = (float)parent.params.gap * (childsCount-1);

    float totalFlex = 0.0f;
    for (const auto& child: parent.childs)
      totalFlex += child.params.flex;

    const float flexSize = (parent.sceneParams.size.x - totalGapSize) / totalFlex;

    for (auto& child: parent.childs)
    {
      const float childWidth = flexSize * child.params.flex;
      child.sceneParams.size = float2{childWidth, parent.sceneParams.size.y};

      child.sceneParams.fontSize = child.params.fontSize.convert(parent.sceneParams.size, m_ScreenSize);

      const float2 parentPos{
        parent.sceneParams.pos.x + xOffset,
        parent.sceneParams.pos.y};

      const float2 childPos = parentPos + float2{child.params.pos};
      child.sceneParams.pos = childPos;

      xOffset += childWidth + (float)parent.params.gap;
      placeChilds(child);
    }
  }

  void ScenePlacer::placeChildsInColumn(Element& parent) const
  {
    float yOffset = 0.0f;
    for (auto& child: parent.childs)
    {
      child.sceneParams.size = float2{
        child.params.size[0].convert(parent.sceneParams.size, m_ScreenSize),
        child.params.size[1].convert(parent.sceneParams.size, m_ScreenSize)
      };

      child.sceneParams.fontSize = child.params.fontSize.convert(parent.sceneParams.size, m_ScreenSize);

      const float2 parentPos{
        parent.sceneParams.pos.x,
        parent.sceneParams.pos.y + yOffset};

      const float relHalign = getRelativeHalignPos(parent.sceneParams.size.x, child);

      const float2 childPos = parentPos + float2{relHalign, 0.0f} + float2{child.params.pos};
      child.sceneParams.pos = childPos;

      yOffset += child.sceneParams.size.y + (float)parent.params.gap;
      placeChilds(child);
    }
  }

  void ScenePlacer::placeChildsInFlexColumn(Element& parent) const
  {
    if (parent.childs.empty())
      return;

    float yOffset = 0.0f;
    const size_t childsCount = parent.childs.size();

    const float totalGapSize = (float)parent.params.gap * (childsCount-1);

    float totalFlex = 0.0f;
    for (const auto& child: parent.childs)
      totalFlex += child.params.flex;

    const float flexSize = (parent.sceneParams.size.y - totalGapSize) / totalFlex;

    for (auto& child: parent.childs)
    {
      const float childHeight = flexSize * child.params.flex;
      child.sceneParams.size = float2{parent.sceneParams.size.x, childHeight};

      child.sceneParams.fontSize = child.params.fontSize.convert(parent.sceneParams.size, m_ScreenSize);

      const float2 parentPos{
        parent.sceneParams.pos.x,
        parent.sceneParams.pos.y + yOffset};

      const float2 childPos = parentPos + float2{child.params.pos};
      child.sceneParams.pos = childPos;

      yOffset += childHeight + (float)parent.params.gap;
      placeChilds(child);
    }
  }

  void ScenePlacer::absPlaceChild(const float2 parent_pos, const float2 parent_size, Element& child) const
  {
    child.sceneParams.size = float2{
      child.params.size[0].convert(parent_size, m_ScreenSize),
      child.params.size[1].convert(parent_size, m_ScreenSize)
    };

    child.sceneParams.fontSize = child.params.fontSize.convert(parent_size, m_ScreenSize);

    const float2 relAnchor = getAnchorPos(parent_pos,  parent_size, child);
    const float2 relPos = getRelativePos(child);
    child.sceneParams.pos = relAnchor + relPos;

    placeChilds(child);
  }

  float2 ScenePlacer::getAnchorPos(const float2 parent_pos, const float2 parent_size, const Element& child) const
  {
    const float2 relPos {
      getRelativeHalignPos(parent_size.x, child),
      getRelativeValignPos(parent_size.y, child)
    };

    return parent_pos + relPos;
  }

  float ScenePlacer::getRelativeHalignPos(const float parent_width, const Element& child) const
  {
    switch (child.params.halign)
    {
      case HorAlignType::Left: return 0.0f;
      case HorAlignType::Right: return parent_width - child.sceneParams.size[0];
      case HorAlignType::Center: return (parent_width - child.sceneParams.size[0]) / 2.0f;
      default: return 0.0;
    }
  }

  float ScenePlacer::getRelativeValignPos(const float parent_height, const Element& child) const
  {
    switch (child.params.valign)
    {
      case VerAlignType::Top: return 0.0f;
      case VerAlignType::Bottom: return parent_height - child.sceneParams.size[1];
      case VerAlignType::Center: return (parent_height - child.sceneParams.size[1]) / 2.0f;
      default: return 0.0;
    }
  }

  float2 ScenePlacer::getRelativePos(const Element& child) const
  {
    float2 pos{child.params.pos};
    
    pos.x = child.params.halign == HorAlignType::Right ? -pos.x : pos.x;
    pos.y = child.params.valign == VerAlignType::Bottom ? -pos.y : pos.y;

    return pos;
  }
}
