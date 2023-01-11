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
    ExportFuncSignature fn = nullptr;
    const char* name;
    uint8_t length;
  };

  using ExportClassOnDestroySignature = void (*)(JSRuntime*, JSValue);

  struct ExportClass
  {
    ExportClass(const char* name)
      : name(name)
    {
    }

    ExportClass&& setConstructor(uint8_t length, ExportFuncSignature fn)
    {
      constructor = {
        .fn = fn,
        .name = "constructor",
        .length = length
      };

      return std::move(*this);
    }

    ExportClass&& setDestroyCb(const ExportClassOnDestroySignature fn)
    {
      onDestroy = fn;
      return std::move(*this);
    }

    const char* name;
    ExportFunc constructor;
    ExportClassOnDestroySignature onDestroy = nullptr;
  };

  class ModuleDescription
  {
    friend VM;

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

      inline void addClass(ExportClass&& c)
      {
        m_Classes.push_back(std::move(c));
      }

    private:
      static int moduleInitFunc(JSContext *ctx, JSModuleDef *m);

      void addExports(JSContext*);
      void setExports(JSContext*);
      void setClassExport(JSContext*, const ExportClass&);

    private:
      JSModuleDef* m_Module;
      eastl::vector<ExportParam> m_Params;
      eastl::vector<ExportFunc> m_Funcs;
      eastl::vector<ExportClass> m_Classes;
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