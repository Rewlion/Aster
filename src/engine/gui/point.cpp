#include "point.h"

namespace Engine::gui
{
  float PointValue::convert(const float2& parent_size, const float2& screen_size) const
  {
    switch (m_Type)
    {
      case Type::General:      return m_Value;
      case Type::ScreenWidth:  return screen_size.x * m_Value / 100.0f;
      case Type::ScreenHeight: return screen_size.y * m_Value / 100.0f;
      case Type::ParentWidth:  return parent_size.x * m_Value / 100.0f;
      case Type::ParentHeight: return parent_size.y * m_Value / 100.0f;
      default: return 0.0f;
    }
  }
}
