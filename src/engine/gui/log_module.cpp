#include <engine/log.h>
#include <engine/qjs/inc.h>
#include <engine/qjs/module_registration.h>
#include <engine/qjs/value.hpp>

namespace Engine::gui
{ 
  static JSValue ui_logerror(JSContext *ctx, JSValueConst this_val,
                             int argc, JSValueConst *argv)
  {
    if (argc != 1)
    {
      JS_ThrowReferenceError(ctx, "logerror function accepts only 1 arg (string), provided: %d", argc);
      return JS_EXCEPTION;
    }
    qjs::ValueView text{argv[0], ctx};

    if (!text.isString())
    {
      JS_ThrowReferenceError(ctx, "logerror expects string as argument");
      return JS_EXCEPTION;
    }

    logerror("ui: {}", text.as<string>());

    return JS_UNDEFINED;
  }

  static const JSCFunctionListEntry ui_funcs[] = {
    JS_CPP_FUNC_DEF("logerror", 1, ui_logerror)
  };

  class LogModule: public qjs::ModuleDescription
  {
    public:
      LogModule()
      {
        addFunc(ui_logerror, "logerror", 1);
      }

      virtual const char* getName() const override
      {
        return "@log";
      }
  };

  REGISTER_MODULE(LogModule);
}
