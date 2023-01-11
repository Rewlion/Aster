#include "constants.h"
#include "gui.h"

#include <engine/qjs/module_registration.h>
#include <engine/qjs/inc.h>
#include <engine/log.h>

namespace Engine::gui
{
  static JSValue ui_add_timer(JSContext *ctx, JSValueConst this_val,
                              int argc, JSValueConst *argv)
  {
    if (argc != 3)
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: argc != 3 (name, sec, cb)");
      return JS_EXCEPTION;
    }

    const qjs::ValueView name{argv[0], ctx};
    const qjs::ValueView sec{argv[1], ctx};
    const qjs::ValueView cb{argv[2], ctx};

    if (!name.isString())
    {
      JS_ThrowReferenceError(ctx,"ui: addTimer #1 argument(name) is not string");
      return JS_EXCEPTION;
    }

    if (!sec.isNumber())
    {
      JS_ThrowReferenceError(ctx,"ui: addTimer: #2 argument(sec) is not a number");
      return JS_EXCEPTION;
    }

    if (!cb.isFunction())
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: #3 argument(cb) is not a function");
      return JS_EXCEPTION;
    }

    if (cb.getLength() != 0)
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: cb shouldn't have any arguments");
      return JS_EXCEPTION;
    }

    gui::manager.getTimersPool().addTimer(
      name.as<string>(),
      cb.duplicate(),
      sec.as<float>()
    );

    return JS_UNDEFINED;
  }

  static JSValue ui_remove_timer(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv)
  {
    if (argc != 1)
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: argc != 1 (name)");
      return JS_EXCEPTION;
    }

    const qjs::ValueView name{argv[0], ctx};
    gui::manager.getTimersPool().removeTimer(name.as<string>());
    return JS_UNDEFINED;
  }

  class StateClass
  {
    public:
      static constexpr const char* name = "State";

      static void onDestroy(JSRuntime* rt, JSValue v)
      {
      }

      static JSValue constructor(JSContext *ctx, JSValueConst this_val,
                                 int argc, JSValueConst *argv)
      {
        JSRuntime* rt = JS_GetRuntime(ctx);
        const auto* rtState = reinterpret_cast<qjs::RuntimeState*>(JS_GetRuntimeOpaque(rt));

        JSClassID classId = rtState->classNameToIdsMap.find(name)->second;
        JSValue prototype = JS_GetClassProto(ctx, classId);
        JSValue obj = JS_NewObjectProtoClass(ctx, prototype, classId);
        JS_FreeValue(ctx, prototype);

        return obj;
      }

      static qjs::ExportClass describe()
      {
        return qjs::ExportClass{name}
          .setConstructor(0, StateClass::constructor)
          .setDestroyCb(StateClass::onDestroy);
      }
  };

  class UiModule: public qjs::ModuleDescription
  {
    public:
      UiModule()
      {
        addParam("RENDER_BOX", (int)RenderType::Box);

        addParam("HALIGN_CENTER", (int)HorAlignType::Center);
        addParam("HALIGN_LEFT", (int)HorAlignType::Left);
        addParam("HALIGN_RIGHT", (int)HorAlignType::Right);

        addParam("VALIGN_CENTER", (int)VerAlignType::Center);
        addParam("VALIGN_TOP", (int)VerAlignType::Top);
        addParam("VALIGN_BOTTOM", (int)VerAlignType::Bottom);

        addParam("FLOW_ROW", (int)Flow::Row);
        addParam("FLOW_COLUMN", (int)Flow::Column);
        addParam("FLOW_FLEX_ROW", (int)Flow::FlexRow);
        addParam("FLOW_FLEX_COLUMN", (int)Flow::FlexColumn);

        addFunc(ui_add_timer, "addTimer", 3);
        addFunc(ui_remove_timer, "removeTimer", 1);

        addClass(StateClass::describe());
      }

      virtual const char* getName() const override
      {
        return "@ui";
      }
  };

  REGISTER_MODULE(UiModule);
}
