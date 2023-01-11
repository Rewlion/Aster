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
      int64_t getLength() const;
      Value duplicate() const;

      inline operator bool() const
      {
        return !(isException() || isUndefined() || isUninitialized() || isNull());
      }

      template<class T>
      T as() const
      {
        static_assert(!"unsupported type for conversion");
        return T{};
      }

      template<> ObjectView as() const { return asObjectView(); }
      template<> ArrayView as() const { return asArrayView(); }
      template<> int as() const { return asInt(); }
      template<> float as() const { return asFloat(); }
      template<> double as() const { return asDouble(); }
      template<> FunctionView as() const { return asFunctionView(); }
      template<> string as() const { return asString(); }

      ValueDump dump();

    protected:
      ObjectView asObjectView() const;
      ArrayView asArrayView() const;
      FunctionView asFunctionView() const;
      string asString() const;
      bool asBool() const;
      int asInt() const;
      float asFloat() const;
      double asDouble() const;

    protected:
      JSValue m_JsValue;
      JSContext* m_Ctx;
  };

  class ValueView: public Value
  {
    public:
      ValueView();
      ValueView(const JSValue, JSContext*);
      ValueView(const ValueView&);
      ValueView(ValueView&&);
      ~ValueView();
      ValueView& operator=(ValueView&&);
  };
}
