#include "array.h"
#include "inc.h"
#include "value.hpp"

namespace qjs
{
  ArrayView::ArrayView(const Value& v)
    : m_Value(v)
  {
    int64_t length = 0;
    JS_GetPropertyLength(m_Value.m_Ctx, &length, v.m_JsValue);
    m_Length = length;
  }

  ArrayView::ArrayView(ArrayView&& rvl)
    : m_Value(rvl.m_Value)
    , m_Length(rvl.m_Length)
  {
    rvl.m_Length = 0;
  }

  Value ArrayView::operator[](const uint32_t i) const
  {
    return Value{
      m_Value.m_Ctx,
      JS_GetPropertyUint32(m_Value.m_Ctx, m_Value.m_JsValue, i)
    };
  }
}
