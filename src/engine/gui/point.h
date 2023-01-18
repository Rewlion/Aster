#pragma once

#include <engine/types.h>

namespace Engine::gui
{
  class PointValue
  {
    public:
      enum class Type : uint8_t
      {
        General,
        ScreenWidth,
        ScreenHeight,
        ParentWidth,
        ParentHeight,
        LAST
      };

      PointValue() = default;

      inline PointValue(float v, Type type)
        : m_Value(v)
        , m_Type(type)
      {
      }

      float convert(const float2& parent_size, const float2& screen_size) const;

    private:
      float m_Value = 0.0f;
      Type m_Type = Type::General;
  };
}
