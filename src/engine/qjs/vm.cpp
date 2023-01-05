#include "vm.h"
#include "exception.h"
#include "inc.h"
#include "quickjs/quickjs.h"
#include "value.hpp"

#include "engine/qjs/module_registration.h"
#include <engine/log.h>
#include <engine/utils/fs.h>

namespace qjs
{
  JSModuleDef* js_module_loader(JSContext *ctx,
                                const char *module_name, void *opaque)
  {
    JSModuleDef *m;
    string src = Utils::read_file_as_string(module_name);

    if (src.empty())
    {
      JS_ThrowReferenceError(ctx, "ui: could not load module '%s'", module_name);
      return nullptr;
    }

    JSValue func_val = JS_Eval(ctx, src.data(), src.size(), module_name,
                              JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
    
    if (JS_IsException(func_val))
      return NULL;
    
    //copied from quickjs-libc
    //js_module_loader
    //it's absolutly unclear, why we should free value if we are getting ptr

    /* the module is already referenced, so we must free it */
    m = reinterpret_cast<JSModuleDef*>(JS_VALUE_GET_PTR(func_val));
    JS_FreeValue(ctx, func_val);

    return m;
  }

  void VM::init()
  {
    m_Rt = JS_NewRuntime();
    m_Ctx = JS_NewContext(m_Rt);

    addPrecreatedModules();

    js_std_add_helpers(m_Ctx, 0, NULL);
    JS_SetModuleLoaderFunc(m_Rt, NULL, js_module_loader, NULL);
  }

  void VM::destroy()
  {
    JS_FreeContext(m_Ctx);
    JS_FreeRuntime(m_Rt); 
  }

  void VM::pushCurrentFileName(std::string_view name)
  {
    m_FileNamesStack.push(name);
  }

  void VM::popCurrentFileName()
  {
    m_FileNamesStack.pop();
  }

  Value VM::eval(std::string_view code)
  {
    const std::string_view file = getCurrentFileName();
    JSValue jsVal = JS_Eval(m_Ctx, code.data(), code.size(), file.data(), JS_EVAL_TYPE_MODULE);
    Value val{jsVal, m_Ctx};

    if (val.isException())
      logerror_exception(val.produceException());

    return val;
  }

  Value VM::getGlobalObjectProperty(std::string_view name)
  {
    Value global{JS_GetGlobalObject(m_Ctx), m_Ctx};
    return global.as<ObjectView>().getProperty(name);
  }

  void VM::addPrecreatedModules()
  {
    const auto addModule = 
    [this](qjs::ModuleDescription* desc) {
      loginfo("ui: adding {} module.", desc->getName());
      desc->addExports(m_Ctx);
    };

    qjs::ModuleRegistration::forEach(addModule);
  }
}
