#pragma once

#include "types.h"

#include <EASTL/vector.h>

#include <EASTL/functional.h>

namespace Engine::ECS
{
  struct QueryComponentDescription
  {
    component_type_id typeId = INVALID_COMPONENT_TYPE_ID;
    component_name_id nameId = INVALID_COMPONENT_NAME_ID;
  };

  using QueryCb = void(*)(class ComponentsAccessor&);
  using EventQueryCb = void(*)(struct Event*, class ComponentsAccessor&);
  using DirectQueryCb = eastl::function<void(class ComponentsAccessor&)>;
  using QueryComponents = eastl::vector<QueryComponentDescription>;
  using DesiredArchetypes = eastl::vector<archetype_id>;

  struct QueryDescription
  {
    QueryCb cb;
    QueryComponents components;
    EventQueryCb eventCb;
    event_hash_name event = INVALID_HASH;
  };

  class SystemRegistration
  {
    friend class Registry;
    public:
      SystemRegistration(QueryCb&& cb, QueryComponents&& comps);

      template<class T>
      static void enumerate(T cb)
      {
        const SystemRegistration* p = m_List;
        while(p)
        {
          cb(*p);
          p = p->m_Next;
        }
      }
    private:
      static SystemRegistration* m_List;
      SystemRegistration* m_Next;

      QueryCb m_Cb;
      QueryComponents m_Comps;
  };

  class EventSystemRegistration
  {
    friend class Registry;
    public:
      EventSystemRegistration(EventQueryCb&& cb, event_hash_name event, QueryComponents&& components);

      template<class T>
      static void enumerate(T cb)
      {
        EventSystemRegistration* p = m_List;
        while(p)
        {
          cb(*p);
          p = p->m_Next;
        }
      }
    private:
      static EventSystemRegistration* m_List;
      EventSystemRegistration* m_Next;

      EventQueryCb m_Cb;
      event_hash_name m_Event;
      QueryComponents m_Comps;
  };

  struct RegisteredQueryInfo
  {
    QueryCb cb;
    DesiredArchetypes archetypes;
  };

  struct RegisteredEventQueryInfo
  {
    EventQueryCb eventCb;
    DesiredArchetypes archetypes;
  };

  struct DirectQueryDescription
  {
    QueryComponents components;
  };

  struct DirectQuery
  {
    QueryComponents components;
    DesiredArchetypes desiredArchetypes;
  };
}

#define DESCRIBE_QUERY_COMPONENT(componentName, componentType)\
  Engine::ECS::QueryComponentDescription\
  {\
    .typeId = str_hash(#componentType),\
    .nameId = str_hash(componentName)\
  }
