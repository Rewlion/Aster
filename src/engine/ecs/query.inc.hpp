#pragma once

namespace ecs
{
  template<class T>
  void SystemRegistration::enumerate(T cb)
  {
    const SystemRegistration* p = m_List;
    while(p)
    {
      cb(*p);
      p = p->m_Next;
    }
  }

  template<class T>
  void DirectQueryRegistration::enumerate(T cb)
  {
    const DirectQueryRegistration* p = m_List;
    while(p)
    {
      cb(*p);
      p = p->m_Next;
    }
  }

  template<class T>
  void EventSystemRegistration::enumerate(T cb)
  {
    EventSystemRegistration* p = m_List;
    while(p)
    {
      cb(*p);
      p = p->m_Next;
    }
  }
}
