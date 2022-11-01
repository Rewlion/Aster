#include "query.h"

namespace Engine::ECS
{
  SystemRegistration* SystemRegistration::m_List = nullptr;
  SystemRegistration::SystemRegistration(QueryCb&& cb, QueryComponents&& comps)
  {
    m_Next = m_List;
    m_List = this;

    m_Cb = std::move(cb);
    m_Comps = std::move(comps);
  }

  EventSystemRegistration* EventSystemRegistration::m_List = nullptr;
  EventSystemRegistration::EventSystemRegistration(EventQueryCb&& cb, event_hash_name event, QueryComponents&& components)
  {
    m_Next = m_List;
    m_List = this;

    m_Cb = std::move(cb);
    m_Event = event;
    m_Comps = std::move(components);
  }
}
