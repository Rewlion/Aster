#include "cmd.h"

namespace console
{
  CmdRegistration::CmdRegistration(const char* name,
                      int min_args,
                      int max_args,
                      CmdRegistration::Cb cb)
    : m_Name(name)
    , m_MinArgs(min_args)
    , m_MaxArgs(max_args)
    , m_Cb(cb)
  {
    m_Next = m_List;
    m_List = this;
  }
}
