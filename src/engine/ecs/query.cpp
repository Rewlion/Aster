#include "query.h"

namespace ecs
{
  SystemRegistration* SystemRegistration::m_List = nullptr;
  SystemRegistration::SystemRegistration(QueryCb&& cb,
                                         QueryComponents&& comps,
                                         const char* name)
  {
    m_Next = m_List;
    m_List = this;

    m_Cb = std::move(cb);
    m_Comps = std::move(comps);
    m_Name = name;
  }

  auto DirectQueryRegistration::getId() const -> query_id_t
  {
    return m_Id;
  }
  
  auto DirectQueryRegistration::size() -> size_t
  {
    return m_List != nullptr ? m_List->m_Id+1 : 0;
  }
  
  auto DirectQueryRegistration::getComponents() const
    -> const QueryComponents&
  {
    return m_Comps;
  }

  DirectQueryRegistration* DirectQueryRegistration::m_List = nullptr;
  DirectQueryRegistration::DirectQueryRegistration(QueryComponents&& comps, const char* name)
  {
    m_Next = m_List;
    m_List = this;

    m_Id = m_Next != nullptr ? m_Next->m_Id+1 : 0;
    m_Comps = std::move(comps);
    m_Name = name;
  }

  EventSystemRegistration* EventSystemRegistration::m_List = nullptr;
  EventSystemRegistration::EventSystemRegistration(EventQueryCb&& cb, name_hash_t event, QueryComponents&& components, const char* name)
  {
    m_Next = m_List;
    m_List = this;

    m_Cb = std::move(cb);
    m_Event = event;
    m_Comps = std::move(components);
    m_Name = name;
  }
}
