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

  ValueDump::ValueDump(JSContext& ctx, const JSValue js_val)
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
    : m_Ctx(nullptr)
    , m_JsValue(0)
  {
  }

  Value::Value(JSContext* ctx, JSValue v)
    : m_Ctx(ctx)
    , m_JsValue(v)
  {
  }

  Value::Value(Value&& rvl)
  {
    *this = std::move(rvl);
  }

  Value::~Value()
  {
    if (isValid())
    {
      JS_FreeValue(m_Ctx, m_JsValue);
      m_Ctx = nullptr;
      m_JsValue = 0;
    }
  }

  Value& Value::operator=(Value&& rvl)
  {
    this->~Value();
    m_Ctx = rvl.m_Ctx;
    m_JsValue = rvl.m_JsValue;
    rvl.m_Ctx = nullptr;
    rvl.m_JsValue = 0;

    return *this;
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

  Value::Type Value::getType() const
  {
    if (isBool())
      return Type::Bool;
    if (isInt())
      return Type::Int;
    if (isFloat())
      return Type::Float;
    if (isArray())
      return Type::Array;
    if (isFunction())
      return Type::Function;
    if (isPlainObject())
      return Type::PlainObject;
    if (isObject())
      return Type::Object;
    if (isException())
      return Type::Exception;
    if (isError())
      return Type::Error;
    if (isUndefined())
      return Type::Undefined;
    if (isUninitialized())
      return Type::Uninitialized;
    if (isString())
      return Type::String;
    if (isSymbol())
      return Type::Symbol;
    if (isNull())
      return Type::Null;

    ASSERT(!"unsupported");
    return {};
  }

  Value Value::produceException()
  {
    ASSERT_FMT(isException(), "can't produce exception: JSValue isn't exception");
    return Value{m_Ctx, JS_GetException(m_Ctx)};
  }

  int64_t Value::getLength() const
  {
    int64_t len = 0;
    JS_GetPropertyLength(m_Ctx, &len, m_JsValue);
    return len;
  }

  Value Value::duplicate() const
  {
    JS_DupValue(m_Ctx, m_JsValue);
    return Value{m_Ctx, m_JsValue};
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

  FunctionView Value::asFunctionView() const
  {
    ASSERT_FMT(isFunction(), "JSValue isn't function.");
    return FunctionView{*this};
  }

  string Value::asString() const
  {
    ASSERT_FMT(isString(), "JSValue isn't string");
    const char* str = JS_ToCString(m_Ctx, m_JsValue);
    string r{str};
    JS_FreeCString(m_Ctx, str);
    return r;
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

  int Value::asIntRanged(const int def, const int max) const
  {
    const int v = asInt();
    return v < max ? v : def;
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
    return ValueDump{*m_Ctx, m_JsValue};
  }

  ValueView::ValueView()
    : Value()
  {
  }

  ValueView::ValueView(JSContext* ctx, const JSValue v)
    : Value(ctx, v)
  {
  }

  ValueView::ValueView(const ValueView& rvl)
  {
    m_Ctx = rvl.m_Ctx;
    m_JsValue = rvl.m_JsValue;
  }

  ValueView::ValueView(ValueView&& rvl)
  {
    this->~ValueView();
    std::swap(m_JsValue, rvl.m_JsValue);
    std::swap(m_Ctx, rvl.m_Ctx);
  }

  ValueView::~ValueView()
  {
    m_JsValue = 0;
    m_Ctx = nullptr;
  }

  ValueView& ValueView::operator=(ValueView&& rvl)
  {
    this->~ValueView();
    *this = std::move(rvl);
    return *this;
  }

}
