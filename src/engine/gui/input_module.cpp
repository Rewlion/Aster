#include <engine/log.h>
#include <engine/qjs/inc.h>
#include <engine/qjs/module_registration.h>
#include <engine/window.h>

namespace Engine::gui
{
  static JSValue ui_set_mouse_cursor(JSContext *ctx, JSValueConst this_val,
                                     int argc, JSValueConst *argv)
  {
    if (argc != 1 || !JS_IsBool(argv[0]))
    {
      JS_ThrowReferenceError(ctx, "ui: arg != bool");
      return JS_EXCEPTION;
    }

    Window::show_cursor(JS_ToBool(ctx, argv[0]));

    return JS_UNDEFINED;
  }

  class InputModule: public qjs::ModuleDescription
  {
    public:
      InputModule()
      {
        addFunc(ui_set_mouse_cursor, "showMouseCursor", 1);
      }

      virtual const char* getName() const override
      {
        return "@input";
      }
  };

  REGISTER_MODULE(InputModule);
}
