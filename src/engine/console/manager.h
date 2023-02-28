#pragma once

#include "hash.h"

#include <EASTL/vector_map.h>

namespace console
{
  class CmdRegistration;

  class Manager
  {
    public:
      void init();

    private:
      eastl::vector_map<cmd_hash_t, CmdRegistration*> m_Cmds;
  };
}
