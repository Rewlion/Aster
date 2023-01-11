#pragma once

#include "forward_decl.h"
#include "array.h"
#include "function.h"
#include "object.h"

namespace qjs
{
  class ValueDump
  {
    friend class Value;
    public:
      ~ValueDump();
      ValueDump() = delete;
      ValueDump(const ValueDump& rvl) = delete;
      ValueDump(ValueDump&& rvl);

    inline std::string_view asString() const
    {
      return std::string_view(m_Dump);
    }

    private:
      ValueDump(const JSValue, JSContext& ctx);

    private:
      JSContext& m_Ctx;
      const char* m_Dump;
      bool m_Managed;
  };

  class Value
  {
    friend VM;
    friend class ModuleLoader;
    friend ArrayView;
    friend FunctionView;
    friend ObjectView;

    public:
      Value();
      Value(const JSValue, JSContext*);
      Value(const Value&) = delete;
      Value(Value&&);
      ~Value();
      Value& operator=(Value&&);

      inline bool isValid() const { return (m_JsValue != 0) && (m_Ctx != nullptr); }

      bool isBool() const;
      bool isNumber() const;
      bool isInt() const;
      bool isFloat() const;
      bool isArray() const;
      bool isFunction() const;
      bool isObject() const;
      bool isPlainObject() const; // plain JS object, that is not function nor array nor anything else
      bool isException() const;
      bool isError() const;
      bool isUndefined() const;
      bool isUninitialized() const;
      bool isString() const;
      bool isSymbol() const;
      bool isNull() const;

      Value produceException();

      inline operator bool() const
      {
        return !(isException() || isUndefined() || isUninitialized() || isNull());
      }

      template<class T>
      T as()
      {
        static_assert(!"unsupported type for conversion");
        return T{};
      }

      template<class T>
      T as() const
      {
        static_assert(!"unsupported type for conversion");
        return T{};
      }

      template<>
      ObjectView as()
      {
        return asObjectView();
      }

      template<>
      ObjectView as() const
      {
        return asObjectView();
      }

      template<>
      ArrayView as()
      {
        return asArrayView();
      }

      template<>
      ArrayView as() const
      {
        return asArrayView();
      }

      template<>
      int as()
      {
        return asInt();
      }

      template<>
      int as() const
      {
        return asInt();
      }

      template<>
      float as()
      {
        return asFloat();
      }

      template<>
      double as()
      {
        return asDouble();
      }

      template<>
      FunctionView as()
      {
        return asFunctionView();
      }

      template<>
      FunctionView as() const
      {
        return asFunctionView();
      }

      ValueDump dump();

    private:
      ObjectView asObjectView() const;
      ArrayView asArrayView() const;
      FunctionView asFunctionView() const;
      bool asBool() const;
      int asInt() const;
      float asFloat() const;
      double asDouble() const;

    private:
      JSValue m_JsValue;
      JSContext* m_Ctx;
  };
}
