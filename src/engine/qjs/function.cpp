#include "function.h"
#include "inc.h"
#include "value.hpp"

#include <memory>

namespace qjs
{
  FunctionView::FunctionView(const Value& v)
    : m_Value(v)
  {
    m_ArgsCount = 0;
    JS_GetPropertyLength(m_Value.m_Ctx, &m_ArgsCount, v.m_JsValue);
  }

  Value FunctionView::operator()(ValueView this_obj, const uint argc, const Value* argv) const
  {
    std::unique_ptr<JSValueConst[]> jsArgv;
    
    if (argc != 0)
      jsArgv.reset(new JSValueConst[argc]);

    for (uint i = 0; i < argc; ++i)
      jsArgv[i] = argv[i].m_JsValue;

    JSValue jsThis = this_obj.isValid() ?
      this_obj.m_JsValue :
      JS_UNDEFINED;

    return Value{
      m_Value.m_Ctx,
      JS_Call(m_Value.m_Ctx, m_Value.m_JsValue, jsThis, argc, jsArgv.get())
    };
  }
}
