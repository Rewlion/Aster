#pragma once

namespace Engine::gui
{
  enum class RenderType: int
  {
    None,
    Box,
    LAST
  };

  enum class HorAlignType: int
  {
    None,
    Center,
    Left,
    Right,
    LAST,
  };

  enum class VerAlignType: int
  {
    None,
    Center,
    Top,
    Bottom,
    LAST
  };

  enum class Flow: int
  {
    None,
    Row,
    Column,
    FlexRow,
    FlexColumn,
    LAST
  };
}
