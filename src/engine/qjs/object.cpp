#include "object.h"
#include "value.hpp"
#include "inc.h"

#include <engine/log.h>

namespace qjs
{
  ObjectView::ObjectView(const Value& v)
    : m_Value(v)
  {
  }

  Value ObjectView::getProperty(const std::string_view name)
  {
    return Value(
      JS_GetPropertyStr(m_Value.m_Ctx, m_Value.m_JsValue, name.data()),
      m_Value.m_Ctx);
  }

  float ObjectView::getFloatOr(const std::string_view name, const float def)
  {
    Value v = getProperty(name);

    if (v.isUndefined())
      return def;

    if (!v.isNumber())
    {
      logerror("ui: {} is not float", name);
      return def;
    }
    return v.as<float>();
  }

  int ObjectView::getIntOr(const std::string_view name, const int def)
  {
    Value v = getProperty(name);

    if (v.isUndefined())
      return def;

    if (!v.isNumber())
    {
      logerror("ui: {} is not int", name);
      return def;
    }
    return v.as<int>();
  }

  int ObjectView::getRangedIntOr(const std::string_view name, const int def, const int max)
  {
    const int val = getIntOr(name, def);
    if (val >= max)
    {
      logerror("ui: invalid {} value {}", name, val);
      return def;
    }
    return val;
  }
}