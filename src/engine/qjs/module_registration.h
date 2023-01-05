#pragma once

#include "forward_decl.h"

#include <variant>
#include <EASTL/vector.h>

namespace qjs
{
  using ExportValue = std::variant<bool, int, float, const char*>;
  struct ExportParam
  {
    const char* name;
    ExportValue value;
  };

  using ExportFuncSignature = JSValue (*)(JSContext*, JSValue, int, JSValue*);
  struct ExportFunc
  {
    ExportFuncSignature fn;
    const char* name;
    uint8_t length;
  };

  class ModuleDescription
  {
    public:
      virtual const char* getName() const = 0;

      inline void addParam(const char* name, const ExportValue value)
      {
        m_Params.push_back({name, value});
      }

      inline void addFunc(ExportFuncSignature fn, const char* name, uint8_t length)
      {
        m_Funcs.push_back({fn, name, length});
      }

      void addExports(JSContext*);
      void setExports(JSContext*);

    private:
      JSModuleDef* m_Module;
      eastl::vector<ExportParam> m_Params;
      eastl::vector<ExportFunc> m_Funcs;
  };

  class ModuleRegistration
  {
    public:
      ModuleRegistration(ModuleDescription*);

      template<class T>
      static void forEach(T cb)
      {
        const ModuleRegistration* p = m_List;
        while(p)
        {
          cb(p->m_Description);
          p = p->m_Next;
        }
      }

    private:
      static ModuleRegistration* m_List;
      ModuleRegistration* m_Next;

      ModuleDescription* m_Description;
  };
}

#define REGISTER_MODULE(module) static qjs::ModuleRegistration module_reg ## __LINE__ {new module}