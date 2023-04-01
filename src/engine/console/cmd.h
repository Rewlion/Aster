#pragma once

#include <engine/types.h>
#include <engine/utils/macros.h>

#include <EASTL/span.h>
#include <EASTL/vector.h>

namespace console
{
  class CmdRegistration
  {
    friend class Manager;
    using Cb = void(*)(eastl::span<string_view>);

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

#define CONSOLE_CMD(name, min_args, max_args, cb) console::CmdRegistration NAME_WITH_LINE(cmdReg)(name, min_args, max_args, cb)