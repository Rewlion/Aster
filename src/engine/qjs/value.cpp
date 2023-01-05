#include "value.hpp"
#include "engine/qjs/forward_decl.h"
#include "inc.h"
#include "quickjs/quickjs.h"

#include <engine/assert.h>
#include <engine/log.h>

namespace qjs
{
  ValueDump::ValueDump(ValueDump&& rvl)
    : m_Ctx(rvl.m_Ctx)
  {
    this->~ValueDump();
    std::swap(m_Dump, rvl.m_Dump);
    std::swap(m_Managed, rvl.m_Managed);
  }

  ValueDump::ValueDump(const JSValue js_val, JSContext& ctx)
    : m_Ctx(ctx)
  {
    m_Dump = JS_ToCString(&m_Ctx, js_val);

    if (m_Dump)
    {
      m_Managed = true;
    }
    else
    {
      m_Managed = false;
      m_Dump = "[exception]";
    }
  }

  ValueDump::~ValueDump()
  {
    if (m_Dump && m_Managed)
    {
      JS_FreeCString(&m_Ctx, m_Dump);
      m_Dump = nullptr;
    }
  }

  Value::Value()
    : m_JsValue(0)
    , m_Ctx(nullptr)
  {
  }

  Value::Value(JSValue v, JSContext* ctx)
    : m_JsValue(v)
    , m_Ctx(ctx)
  {
  }

  Value::Value(Value&& rvl)
  {
    this->~Value();
    m_JsValue = rvl.m_JsValue;
    m_Ctx = rvl.m_Ctx;
    rvl.m_JsValue = 0;
    rvl.m_Ctx = nullptr;
  }

  Value::~Value()
  {
    if (isValid())
    {
      JS_FreeValue(m_Ctx, m_JsValue);
      m_JsValue = 0;
      m_Ctx = nullptr;
    }
  }

  bool Value::isBool() const
  {
    return JS_IsBool(m_JsValue);
  }

  bool Value::isNumber() const
  {
    return JS_IsNumber(m_JsValue);
  }

  bool Value::isInt() const
  {
    const int tag = JS_VALUE_GET_TAG(m_JsValue);
    return tag == JS_TAG_INT;
  }

  bool Value::isFloat() const
  {
    const int tag = JS_VALUE_GET_TAG(m_JsValue);
    return JS_TAG_IS_FLOAT64(tag);
  }

  bool Value::isArray() const
  {
    return JS_IsArray(m_Ctx, m_JsValue);
  }

  bool Value::isFunction() const
  {
    return JS_IsFunction(m_Ctx, m_JsValue);
  }

  bool Value::isObject() const
  {
    return JS_IsObject(m_JsValue);
  }

  bool Value::isPlainObject() const
  {
    return JS_IsObjectPlain(m_Ctx, m_JsValue);
  }

  bool Value::isException() const
  {
    return JS_IsException(m_JsValue);
  }

  bool Value::isError() const
  {
    return JS_IsError(m_Ctx, m_JsValue);
  }

  bool Value::isUndefined() const
  {
    return JS_IsUndefined(m_JsValue);
  }

  bool Value::isUninitialized() const
  {
    return JS_IsUninitialized(m_JsValue);
  }

  bool Value::isString() const
  {
    return JS_IsString(m_JsValue);
  }

  bool Value::isSymbol() const
  {
    return JS_IsSymbol(m_JsValue);
  }

  bool Value::isNull() const
  {
    return JS_IsNull(m_JsValue);
  }

  Value Value::produceException()
  {
    ASSERT_FMT(isException(), "can't produce exception: JSValue isn't exception");
    return Value{JS_GetException(m_Ctx), m_Ctx};
  }

  ObjectView Value::asObjectView() const
  {
    ASSERT_FMT(isObject(), "JSValue isn't object");
    return ObjectView{*this};
  }

  ArrayView Value::asArrayView() const
  {
    ASSERT_FMT(isObject(), "JSValue isn't array");
    return ArrayView{*this};
  }

  bool Value::asBool() const
  {
    ASSERT_FMT(isBool(), "JSValue isn't bool");
    return JS_ToBool(m_Ctx, m_JsValue);
  }

  int Value::asInt() const
  {
    ASSERT_FMT(isNumber(), "JSValue isn't int");
    int v = 0;
    JS_ToInt32(m_Ctx,&v, m_JsValue);
    return v;
  }

  float Value::asFloat() const
  {
    ASSERT_FMT(isNumber(), "JSValue isn't float");
    double v = 0.0;
    JS_ToFloat64(m_Ctx, &v, m_JsValue);
    return v;
  }

  double Value::asDouble() const
  {
    ASSERT_FMT(isNumber(), "JSValue isn't float");
    double v = 0.0;
    JS_ToFloat64(m_Ctx, &v, m_JsValue);
    return v;
  }

  ValueDump Value::dump()
  {
    return ValueDump{m_JsValue, *m_Ctx};
  }

}
