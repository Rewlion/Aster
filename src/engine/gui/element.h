#pragma once

#include "constants.h"

#include <engine/types.h>

#include <EASTL/vector.h>

#include <array>
#include <variant>

namespace Engine::gui
{
  using SizeParam = std::array<int, 2>;

  union ColorParam
  {
    struct
    {
      uint8_t b,g,r,a;
    } val;
    uint32_t raw;

    ColorParam(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    {
      val.r = r;
      val.g = g;
      val.b = b;
      val.a = a;
    }
    ColorParam(const uint8_t r, const uint8_t g, const uint8_t b)
    {
      val.r = r;
      val.g = g;
      val.b = b;
      val.a = 255;
    }
    ColorParam(const uint32_t color)
    {
      raw = color;
    }
    ColorParam()
    {
      raw = 0;
    }

    inline float4 getRenderColor() const
    {
      return float4{
        (float)val.r / 255.0f,
        (float)val.g / 255.0f,
        (float)val.b / 255.0f,
        (float)val.a / 255.0f,
      };
    }
  };

  struct Element
  {
    struct Params
    {
      SizeParam          size;
      int2               pos;
      int                gap;
      float              flex;
      Flow               flow;
      ColorParam         color;
      RenderType         render;
      HorAlignType       halign;
      VerAlignType       valign;
    };

    struct SceneParams
    {
      float2 pos = {0, 0};
      float2 size = {0, 0};
    };

    Params params;
    SceneParams sceneParams;

    eastl::vector<Element> childs;
  };
}
