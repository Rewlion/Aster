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

      static JSClassID getClassID(JSRuntime* rt)
      {
        const auto* rtState = reinterpret_cast<qjs::RuntimeState*>(JS_GetRuntimeOpaque(rt));

        JSClassID classId = rtState->classNameToIdsMap.find(name)->second;
        return classId;
      }

      static JSValue constructFromPrototype(JSContext* ctx)
      {
        JSRuntime* rt = JS_GetRuntime(ctx);
        JSClassID classId = getClassID(rt);
        JSValue prototype = JS_GetClassProto(ctx, classId);
        JSValue obj = JS_NewObjectProtoClass(ctx, prototype, classId);
        JS_FreeValue(ctx, prototype);

        return obj;
      }

      static JSValue constructor(JSContext* ctx, JSValueConst this_val,
                                 int argc, JSValueConst* argv)
      {
        JSValue obj = constructFromPrototype(ctx);
        ReactStateClass* wrapper = new ReactStateClass(obj, ctx);
        JS_SetOpaque(obj, wrapper);

        return obj;
      }

      static void onDestroy(JSRuntime* rt, JSValue obj)
      {
        ReactStateClass* wrapper = unpack(rt, obj);
        delete wrapper;
        JS_SetOpaque(obj, nullptr);
      }

      static qjs::ExportClass describe()
      {
        return qjs::ExportClass{name}
          .setConstructor(0, ReactStateClass::constructor)
          .setDestroyCb(ReactStateClass::onDestroy);
      }

      static ReactStateClass* unpack(JSContext* ctx, const JSValue obj)
      {
        return unpack(JS_GetRuntime(ctx), obj);
      }

      static ReactStateClass* unpack(JSRuntime* rt, const JSValue obj)
      {
        const JSClassID classId = getClassID(rt);
        return reinterpret_cast<ReactStateClass*>(JS_GetOpaque(obj, classId));
      }

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
