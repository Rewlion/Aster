#pragma once

#include "constants.h"
#include "point.h"
#include "react_state_registration.h"

#include <engine/qjs/value.hpp>
#include <engine/types.h>

#include <EASTL/vector.h>

#include <array>
#include <memory>
#include <variant>

namespace Engine::gui
{
  using SizeParam = std::array<PointValue, 2>;

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

  class IBehavior;
  using BehaviorsArray = eastl::vector<IBehavior*>;
  using ObservedStates = eastl::vector<ReactStateRegistration>;
  struct Element
  {
    using Ptr = std::unique_ptr<Element>;
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
      bool               clipChilds;
      string             text;
      HorAlignType       textHalign;
      VerAlignType       textValign;
      PointValue         fontSize;
    };

    struct DynamicParams
    {
      ObservedStates     observes;
      qjs::Value         constructor;
      BehaviorsArray     behaviors;
      qjs::Value         observeBtnState;
      qjs::Value         onClick;
    };

    struct SceneParams
    {
      float2 pos = {0, 0};
      float2 size = {0, 0};
      float fontSize = 1.0;
    };

    size_t id = 0;
    size_t zOrder = 0;

    Params params;
    SceneParams sceneParams;
    DynamicParams dynamicParams;

    qjs::Value object;
    eastl::vector<Element::Ptr> childs;

    inline bool isDynamic() const
    {
      return dynamicParams.constructor.isValid();
    }

    inline bool isInside(float2 pos)
    {
      const float x0 = sceneParams.pos.x;
      const float y0 = sceneParams.pos.y;
      const float w = sceneParams.size.x + x0;
      const float h = sceneParams.size.y + y0;

      return (pos.x >= x0 && pos.x < w) && (pos.y >= y0 && pos.y < h);
    }
  };
}
