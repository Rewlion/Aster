#pragma once

#include "eid.h"
#include "events_queue.hpp"
#include "query.h"
#include "template.h"
#include "entity_component_init.h"
#include "registered_component.h"
#include "archetype.h"

#include <engine/log.h>

#include <EASTL/tuple.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace ecs
{
  class Registry final
  {
    public:
      void addTemplate(const string& name,
                       TemplateParentNames&&,
                       TemplateComponentsMap&&);

      auto createEntity(const string_view tmpl, EntityComponents&& comps) -> EntityId;
      auto destroyEntity(const EntityId eid) -> bool;

      template<BasedFromEcsEvent T>
      void broadcastEvent(T&& event);

      template<BasedFromEcsEvent T>
      void registerEvent();

      void query(const query_id_t queryId, DirectQueryCb cb);

      void tick();

    private:
      void registerCppQueries();
      void registerSystem(const QueryCb& cb,
                          const QueryComponents& components);
      void registerEventSystem(const EventQueryCb& cb,
                               const name_hash_t event,
                               const QueryComponents& components);

      void processEvents();

      auto findDesiredArchetypes(const QueryComponents&) -> eastl::vector<archetype_id_t>;

      void queryArchetype(const archetype_id_t, QueryCb);
      void queryArchetype(const archetype_id_t, const DirectQueryCb&);
      void queryArchetypeByEvent(Event*, const archetype_id_t, EventQueryCb);
      void processEventWithoutArchetypes(Event*, EventQueryCb);

    private:
      struct EntityInfo
      {
        EntityId eid;
        chunk_id_t chunkId;
        chunk_eid_t chunkEid;
      };

      Templates m_Templates;
      RegisteredComponents m_RegisteredComponents;
      Archetypes m_Archetypes;
      eastl::vector_map<uint64_t, EntityInfo> m_EntityInfosMap;
    
      eastl::vector_map<name_hash_t, eastl::vector<RegisteredEventQueryInfo>> m_EventHandleQueries;
      eastl::vector<RegisteredQueryInfo> m_RegisteredQueues;
      eastl::vector<DirectQuery> m_RegisteredDirectQueries;
      
      EventsQueue m_EventsQueue;
  };
}

#include "registry.inc.hpp"
