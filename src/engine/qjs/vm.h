#pragma once

#include "forward_decl.h"

#include <EASTL/stack.h>

#include <string_view>

namespace qjs
{
  class VM
  {
    public:
      void init();
      void destroy();
      
      void pushCurrentFileName(std::string_view name);
      void popCurrentFileName();
      Value eval(std::string_view code);
      Value getGlobal();
      Value getGlobalObjectProperty(std::string_view name);

    private:
      void addPrecreatedModules();

      inline std::string_view getCurrentFileName() const
      {
        if (!m_FileNamesStack.empty())
          return m_FileNamesStack.top();
        return "";
      }

    private:
      eastl::stack<std::string_view> m_FileNamesStack;
      JSRuntime* m_Rt;
      JSContext* m_Ctx;
  };
}
