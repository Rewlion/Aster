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
      JS_ThrowReferenceError(ctx, "ui: addTimer: argc != 3 (name, cb, sec)");
      return JS_EXCEPTION;
    }

    if (!JS_IsString(argv[0]))
    {
      JS_ThrowReferenceError(ctx,"ui: addTimer #1 argument(name) is not string");
      return JS_EXCEPTION;
    }

    if (!JS_IsNumber(argv[1]))
    {
      JS_ThrowReferenceError(ctx,"ui: addTimer: #2 argument(sec) is not a number");
      return JS_EXCEPTION;
    }

    if (!JS_IsFunction(ctx, argv[2]))
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: #3 argument(cb) is not a function");
      return JS_EXCEPTION;
    }

    int64_t argcCount = 0;
    JS_GetPropertyLength(ctx, &argcCount, argv[2]);
    if (argcCount != 0)
    {
      JS_ThrowReferenceError(ctx, "ui: addTimer: cb shouldn't have any arguments");
      return JS_EXCEPTION;
    }

    const char* nameJs = JS_ToCString(ctx, argv[0]);
    string name{nameJs};
    JS_FreeCString(ctx, nameJs);

    JS_DupValue(ctx, argv[2]);
    qjs::Value cb{argv[2], ctx};

    double sec = 0.0;
    JS_ToFloat64(ctx, &sec, argv[1]);

    gui::manager.getTimersPool().addTimer(name, std::move(cb), sec);

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

    const char* nameJs = JS_ToCString(ctx, argv[0]);
    string name{nameJs};
    JS_FreeCString(ctx, nameJs);

    gui::manager.getTimersPool().removeTimer(name);
    return JS_UNDEFINED;
  }

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
      }

      virtual const char* getName() const override
      {
        return "@ui";
      }
  };

  REGISTER_MODULE(UiModule);
}
