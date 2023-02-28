#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

namespace console
{
  class CmdRegistration
  {
    friend class Manager;
    using Cb = void(*)(eastl::vector<string>);

    public:
      CmdRegistration(const char* name,
                      int min_args,
                      int max_args,
                      Cb cb);

    private:
      template<class Cb>
      static
      void enumerate(Cb cb);

    private:
      static CmdRegistration* m_List;
      CmdRegistration* m_Next;

      const char* m_Name;
      int m_MinArgs;
      int m_MaxArgs;
      Cb m_Cb;
  };
}

#include "cmd.inc.hpp"
