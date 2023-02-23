#pragma once

#include "core_types.h"

#include <EASTL/vector.h>

#include <EASTL/functional.h>

namespace ecs
{
  using QueryCb = void(*)(class ComponentsAccessor&);
  using EventQueryCb = void(*)(struct Event*, class ComponentsAccessor&);
  using DirectQueryCb = eastl::function<void(class ComponentsAccessor&)>;
  struct QueryComponent
  {
    name_hash_t nameHash;
    component_type_id_t typeId;
  };
  using QueryComponents = eastl::vector<QueryComponent>;

  struct QueryDescription
  {
    QueryCb cb;
    QueryComponents components;
    EventQueryCb eventCb;
    name_hash_t event;
  };

  class SystemRegistration
  {
    friend class Registry;
    public:
      SystemRegistration(QueryCb&& cb, QueryComponents&& comps, const char* name);

      template<class T>
      static
      void enumerate(T cb);

    private:
      static SystemRegistration* m_List;
      SystemRegistration* m_Next;

      QueryCb m_Cb;
      QueryComponents m_Comps;
      const char* m_Name;
  };

  class DirectQueryRegistration
  {
    friend class Registry;
    public:
      DirectQueryRegistration(QueryComponents&& comps, const char* name);

      template<class T>
      static
      void enumerate(T cb);

      auto getId() const -> query_id_t;
      
      static
      auto size() -> size_t;
      
      auto getComponents() const -> const QueryComponents&;

    private:
      static DirectQueryRegistration* m_List;
      DirectQueryRegistration* m_Next;

      query_id_t m_Id;
      QueryComponents m_Comps;
      const char* m_Name;
  };

  class EventSystemRegistration
  {
    friend class Registry;
    public:
      EventSystemRegistration(EventQueryCb&& cb,
                              name_hash_t event,
                              QueryComponents&& components,
                              const char* name);

      template<class T>
      static
      void enumerate(T cb);

    private:
      static EventSystemRegistration* m_List;
      EventSystemRegistration* m_Next;

      EventQueryCb m_Cb;
      name_hash_t m_Event;
      QueryComponents m_Comps;
      const char* m_Name;
  };

  struct RegisteredQueryInfo
  {
    QueryCb cb;
    eastl::vector<archetype_id_t> archetypes;
  };

  struct RegisteredEventQueryInfo
  {
    EventQueryCb eventCb;
    eastl::vector<archetype_id_t> archetypes;
  };

  struct DirectQueryDescription
  {
    QueryComponents components;
  };

  struct DirectQuery
  {
    eastl::vector<archetype_id_t> desiredArchetypes;
  };
}

#define DESCRIBE_QUERY_COMPONENT(componentName, componentType)\
  ecs::QueryComponent\
  {\
    .nameHash = compile_ecs_name_hash(componentName),\
    .typeId = CompileTypeId::from<componentType>(),\
  }

#include "query.inc.hpp"
