#include "react_state.h"

#include "runtime_state.h"

namespace Engine::gui
{
  static uint64_t last_registered_state_id = 0;

  JSClassID ReactStateClass::getClassID(JSRuntime* rt)
  {
    const auto* rtState = reinterpret_cast<qjs::RuntimeState*>(JS_GetRuntimeOpaque(rt));

    JSClassID classId = rtState->classNameToIdsMap.find(name)->second;
    return classId;
  }

  JSValue ReactStateClass::constructFromPrototype(JSContext* ctx)
  {
    JSRuntime* rt = JS_GetRuntime(ctx);
    JSClassID classId = getClassID(rt);
    JSValue prototype = JS_GetClassProto(ctx, classId);
    JSValue obj = JS_NewObjectProtoClass(ctx, prototype, classId);
    JS_FreeValue(ctx, prototype);

    return obj;
  }

  JSValue ReactStateClass::constructor(JSContext* ctx, JSValueConst this_val,
                             int argc, JSValueConst* argv)
  {
    JSValue obj = constructFromPrototype(ctx);
    ReactStateClass* wrapper = new ReactStateClass(obj, ctx);
    JS_SetOpaque(obj, wrapper);

    JS_DefinePropertyValueStr(ctx, obj, "__value", JS_UNDEFINED, JS_PROP_WRITABLE);

    const auto getter = 
    [](JSContext* ctx, JSValueConst this_obj) {
      return qjs::ValueView{ctx, this_obj}
              .as<qjs::ObjectView>()
              .getJsProperty("__value");
    };
    const auto setter = 
    [](JSContext* ctx, JSValueConst this_obj, JSValue obj) {
      JS_SetPropertyStr(ctx, this_obj, "__value", obj);
      qjs::get_user_state<RuntimeState>(ctx)
      ->reactStorage.markDirtyState(
        ReactStateClass::unpack(ctx, this_obj));
      return JS_UNDEFINED;
    };

    JSCFunctionListEntry getset[] = {
      JS_CPP_GETSET_DEF("value", getter, setter)
    };

    JS_SetPropertyFunctionList(ctx, obj, getset, 1);

    return obj;
  }

  void ReactStateClass::onDestroy(JSRuntime* rt, JSValue obj)
  {
    ReactStateClass* wrapper = unpack(rt, obj);
    delete wrapper;
    JS_SetOpaque(obj, nullptr);
  }

  ReactStateClass::ReactStateClass(const JSValue obj, JSContext* ctx)
    : m_Id(last_registered_state_id++)
    , m_Obj(ctx, obj)
  {
    qjs::get_user_state<RuntimeState>(ctx)
      ->reactStorage.add(this);
  }

  ReactStateClass::~ReactStateClass()
  {
    qjs::get_user_state<RuntimeState>(m_Obj.getContext())
      ->reactStorage.remove(this);
  }

  qjs::ExportClass ReactStateClass::describe()
  {
    return qjs::ExportClass{name}
      .setConstructor(0, ReactStateClass::constructor)
      .setDestroyCb(ReactStateClass::onDestroy);
  }

  ReactStateClass* ReactStateClass::unpack(JSContext* ctx, const JSValue obj)
  {
    return unpack(JS_GetRuntime(ctx), obj);
  }

  ReactStateClass* ReactStateClass::unpack(JSRuntime* rt, const JSValue obj)
  {
    const JSClassID classId = getClassID(rt);
    return reinterpret_cast<ReactStateClass*>(JS_GetOpaque(obj, classId));
  }

  ReactStateClass* ReactStateClass::unpack(const qjs::Value& v)
  {
    return unpack(v.getContext(), v.getJsValue());
  }
}
