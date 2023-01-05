#include "module_registration.h"
#include "inc.h"

#include <engine/assert.h>
#include <engine/log.h>

#include <EASTL/map.h>

namespace qjs
{
  static eastl::map<JSModuleDef*, ModuleDescription*> module_def_to_description; 

  ModuleRegistration* ModuleRegistration::m_List = nullptr;
  ModuleRegistration::ModuleRegistration(ModuleDescription* desc)
  {
    m_Next = m_List;
    m_List = this;

    m_Description = desc;
  }

  static eastl::vector<JSCFunctionListEntry> get_fn_entries(const eastl::vector<ExportFunc>& fns)
  {
    eastl::vector<JSCFunctionListEntry> fnEntries;
    fnEntries.reserve(fns.size());
    for (const auto& fn: fns)
    {
      fnEntries.push_back(
        JS_CPP_FUNC_DEF(fn.name, fn.length, fn.fn)
      );
    }

    return fnEntries;
  }

  static int module_init_func(JSContext *ctx, JSModuleDef *m)
  {
    const auto it = module_def_to_description.find(m);
    ASSERT(it != module_def_to_description.end());

    ModuleDescription* desc = it->second;
    desc->setExports(ctx);

    return 0;
  }

  void ModuleDescription::addExports(JSContext* ctx)
  {
    m_Module = JS_NewCModule(ctx, getName(), module_init_func);

    if (!m_Module)
    {
      logerror("ui: failed to add {} module.", getName());
      return;
    }

    module_def_to_description.insert({m_Module, this});

    for (const auto& p: m_Params)
      JS_AddModuleExport(ctx, m_Module, p.name);
    
    eastl::vector<JSCFunctionListEntry> fnEntries = get_fn_entries(m_Funcs);
    if (!fnEntries.empty())
      JS_AddModuleExportList(ctx, m_Module, fnEntries.data(), fnEntries.size());
  }

  void ModuleDescription::setExports(JSContext* ctx)
  {
    #define SET_VAL_CASE(type, constr)\
    if (std::holds_alternative<type>(p.value))\
    {\
      JSValue v = constr(ctx, std::get<type>(p.value));\
      JS_SetModuleExport(ctx, m_Module, p.name, v);\
    }

    for (const auto& p: m_Params)
    {
       SET_VAL_CASE(bool, JS_NewBool)
       SET_VAL_CASE(int, JS_NewInt32)
       SET_VAL_CASE(float, JS_NewFloat64)
       SET_VAL_CASE(const char*, JS_NewString)
    }

    #undef SET_VAL_CASE

    eastl::vector<JSCFunctionListEntry> fnEntries = get_fn_entries(m_Funcs);
    if (!fnEntries.empty())
      JS_SetModuleExportList(ctx, m_Module, fnEntries.data(), fnEntries.size());
  }
}
 