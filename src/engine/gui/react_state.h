#pragma once

#include <engine/qjs/inc.h>
#include <engine/qjs/module_registration.h>
#include <engine/qjs/runtime_state.h>
#include <engine/qjs/value.hpp>

namespace Engine::gui
{
  class ReactStateClass
  {
    public:
      static constexpr const char* name = "State";

      static JSClassID        getClassID(JSRuntime* rt);
      static JSValue          constructFromPrototype(JSContext* ctx);
      static JSValue          constructor(JSContext* ctx, JSValueConst this_val,
                                          int argc, JSValueConst* argv);
      static void             onDestroy(JSRuntime* rt, JSValue obj);
      static qjs::ExportClass describe();
      static ReactStateClass* unpack(JSContext* ctx, const JSValue obj);
      static ReactStateClass* unpack(JSRuntime* rt, const JSValue obj);
      static ReactStateClass* unpack(const qjs::Value& v);

    public:
      inline uint64_t getId() const
      {
        return m_Id;
      }

      ~ReactStateClass();

    private:
      ReactStateClass(const JSValue obj, JSContext* ctx);

    private:
      uint64_t m_Id;
      qjs::ValueView m_Obj;
  };
}
