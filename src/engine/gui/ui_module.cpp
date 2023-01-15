#include "constants.h"
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

        addParam("BUTTON_HOVERED", (int)BTN_ST_HOVERED);
        addParam("BEHAVIOR_BUTTON", (int)BehaviorType::Button);

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
