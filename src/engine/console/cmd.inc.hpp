#pragma once

namespace console
{
  template<class Cb>
  void CmdRegistration::enumerate(Cb cb)
  {
    CmdRegistration* p = m_List;
    while(p)
    {
      cb(p);
      p = p->m_Next;
    }
  }
}
