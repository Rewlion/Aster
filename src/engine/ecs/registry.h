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
      auto getEntityTemplateName(const EntityId eid) const -> string_view;

      auto createEntity(const string_view tmpl, EntityComponents&& inits = {}) -> EntityId;
      auto recreateEntity(const EntityId eid, const string_view tmpl, EntityComponents&& inits) -> EntityId;
      void destroyEntity(const EntityId eid);

      template<BasedFromEcsEvent T>
      void broadcastEvent(T&& event);
      template<BasedFromEcsEvent T>
      void broadcastEventSync(T&& event);
      template<BasedFromEcsEvent T>
      void sendEvent(T&& event, const EntityId eid);

      void initialRegistration();

      void query(const query_id_t, DirectQueryCb);
      void query(const EntityId, DirectQueryCb);

      void tick();

    private:
      void registerEvents();
      void registerCppQueries();
      void registerSystem(const QueryCb& cb,
                          const QueryComponents& components);
      void registerEventSystem(const EventQueryCb& cb,
                               const name_hash_t event,
                               const QueryComponents& components);
      void registerEvent(const char* event_name);

      void processDirectEvent(Event*);
      void processBroadcastEvent(Event*);
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
        archetype_id_t archId;
        chunk_id_t chunkId;
        chunk_eid_t chunkEid;
        string templateName;
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

  template<BasedFromEcsEvent T>
  void Registry::broadcastEvent(T&& event)
  {
    if (m_EventHandleQueries.find(event.eventNameHash) != m_EventHandleQueries.end())
      m_EventsQueue.pushEvent(eastl::forward<T>(event));
    else
      logerror("can't broadcast event `{}`, it is not registered.", event.eventNameHash);
  }

  template<BasedFromEcsEvent T>
  void Registry::broadcastEventSync(T&& event)
  {
    if (m_EventHandleQueries.find(event.eventNameHash) == m_EventHandleQueries.end())
      logerror("can't broadcast event `{}`, it is not registered.", event.eventNameHash);

    processBroadcastEvent(&event);
  }

  template<BasedFromEcsEvent T>
  void Registry::sendEvent(T&& event, const EntityId eid)
  {
    event.receiver = eid;
    if (m_EventHandleQueries.find(event.eventNameHash) != m_EventHandleQueries.end())
      m_EventsQueue.pushEvent(eastl::forward<T>(event));
    else
      logerror("can't send event `{}`, it is not registered.", event.eventNameHash);
  }
}
