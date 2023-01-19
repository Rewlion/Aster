#include "constants.h"
#include "point.h"
#include "react_state.h"
#include "runtime_state.h"

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

    const qjs::ValueView name{ctx, argv[0]};
    const qjs::ValueView sec{ctx, argv[1]};
    const qjs::ValueView cb{ctx, argv[2]};

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

    qjs::get_user_state<RuntimeState>(ctx)
      ->timers.add(
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

    const qjs::ValueView name{ctx, argv[0]};
    qjs::get_user_state<RuntimeState>(ctx)
      ->timers.remove(name.as<string>());

    return JS_UNDEFINED;
  }

  static JSValue ui_point_value_base(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv,
                                     const char* func_name, const PointValue::Type type)
  {
    if (argc != 1)
    {
      JS_ThrowReferenceError(ctx, "ui: %s: argc != 1 (value)", func_name);
      return JS_EXCEPTION;
    }

    if (!JS_IsNumber(argv[0]))
    {
      JS_ThrowReferenceError(ctx, "ui: %s argv[0] != number", func_name);
      return JS_EXCEPTION;
    }

    double val = 0.0f;
    JS_ToFloat64(ctx, &val, argv[0]);

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, ret, "value", argv[0]);
    JSValue typeJs = JS_NewInt32(ctx, (int32_t)type);
    JS_SetPropertyStr(ctx, ret, "type", typeJs);
    JS_FreeValue(ctx, typeJs);

    return ret;
  }

  static JSValue ui_sw(JSContext *ctx, JSValueConst this_val,
                       int argc, JSValueConst *argv)
  {
    return ui_point_value_base(ctx, this_val, argc, argv, "sw", PointValue::Type::ScreenWidth);
  }

  static JSValue ui_sh(JSContext *ctx, JSValueConst this_val,
                       int argc, JSValueConst *argv)
  {
    return ui_point_value_base(ctx, this_val, argc, argv, "sh", PointValue::Type::ScreenHeight);
  }

  static JSValue ui_pw(JSContext *ctx, JSValueConst this_val,
                       int argc, JSValueConst *argv)
  {
    return ui_point_value_base(ctx, this_val, argc, argv, "pw", PointValue::Type::ParentWidth);
  }

  static JSValue ui_ph(JSContext *ctx, JSValueConst this_val,
                       int argc, JSValueConst *argv)
  {
    return ui_point_value_base(ctx, this_val, argc, argv, "ph", PointValue::Type::ParentHeight);
  }

  class UiModule: public qjs::ModuleDescription
  {
    public:
      UiModule()
      {
        addParam("RENDER_BOX", (int)RenderType::Box);
        addParam("RENDER_TEXT", (int)RenderType::Text);

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

        addParam("BUTTON_HOVERED", (int)BTN_ST_HOVERED);
        addParam("BUTTON_CLICKED", (int)BTN_ST_CLICKED);
        addParam("BEHAVIOR_BUTTON", (int)BehaviorType::Button);

        addParam("POINT_GENERAL", (int)PointValue::Type::General);
        addParam("POINT_SCREEN_WIDTH", (int)PointValue::Type::ScreenWidth);
        addParam("POINT_SCREEN_HEIGHT", (int)PointValue::Type::ScreenHeight);
        addParam("POINT_PARENT_WIDTH", (int)PointValue::Type::ParentWidth);
        addParam("POINT_PARENT_HEIGHT", (int)PointValue::Type::ParentHeight);

        addFunc(ui_sw, "sw", 1);
        addFunc(ui_sh, "sh", 1);
        addFunc(ui_pw, "pw", 1);
        addFunc(ui_ph, "ph", 1);

        addFunc(ui_add_timer, "addTimer", 3);
        addFunc(ui_remove_timer, "removeTimer", 1);

        addClass(ReactStateClass::describe());
      }

      virtual const char* getName() const override
      {
        return "@ui";
      }
  };

  REGISTER_MODULE(UiModule);
}
