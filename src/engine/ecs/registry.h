#pragma once

#include "archetype.h"
#include "components.h"
#include "eid.h"
#include "entity_cb.h"
#include "events_queue.hpp"
#include "query.h"
#include "types.h"

#include <engine/log.h>

#include <EASTL/tuple.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace Engine::ECS
{
  class EntityInitializer;

  class Registry final
  {
    public:
      void addTemplate(const string& tmpl,
                       eastl::vector<TemplateComponentDescription>&&,
                       const eastl::vector<string>& extends_templates);

      void createEntity(const string_view tmpl, const CreationCb&);
      bool destroyEntity(const EntityId eid);

      template<BasedFromEcsEvent T>
      void broadcastEvent(T&& event);

      template<BasedFromEcsEvent T>
      void registerEvent();

      void query(const query_id queryId, DirectQueryCb cb);

      void tick();

    private:
      struct EntityInfo
      {
        EntityId eid;
        archetype_id archetypeId = INVALID_ARCHETYPE_ID;
        block_id blockId = 0;
        chunk_id chunkId = 0;
      };

    private:
      inline static eastl::vector<DirectQuery>& getDirectQueries();
      void registerCppQueries();
      void registerSystem(const QueryCb& cb, const QueryComponents& components);
      void registerEventSystem(const EventQueryCb& cb, const event_hash_name event, const QueryComponents& components);

      void processEvents();

      DesiredArchetypes findDesiredArchetypes(const QueryComponents& queryComponents);

      void queryArchetype(const archetype_id archetypeId, QueryCb cb);
      void queryArchetype(const archetype_id archetypeId, const DirectQueryCb& cb);
      void queryArchetypeByEvent(Event* event, const archetype_id archetypeId, EventQueryCb cb);
      void processEventWithoutArchetypes(Event* event, EventQueryCb cb);

      eastl::tuple<CompToPlacementMap,size_t>
        placeArchetypeTypes(eastl::vector<ArchetypeComponent>&) const;
      bool mergeArchetypeComponents(eastl::vector<ArchetypeComponent>& to, 
                                    const eastl::vector<ArchetypeComponent>& from) const;
      archetype_id findArchetype(const eastl::vector<ArchetypeComponent>&) const;
      archetype_id makeArchetype(eastl::vector<ArchetypeComponent>&&);
      archetype_id makeArchetype(eastl::vector<ArchetypeComponent>&&,
                                 const eastl::vector<archetype_id>& extends);

      archetype_id mergeTemplates(const string final_name, const template_name_id final_id, const eastl::vector<string>& tmpls);
      EntityComponents makeEntityComponents(const archetype_id) const;
      bool isTemplateRegistered(const template_name_id) const;
      eastl::vector<archetype_id> toArchetypeIds(const eastl::vector<string>& template_names) const;

    private:
      eastl::vector<ArchetypeStorage> m_Archetypes;
      TemplateToArchetypeMap m_TemplateToArchetypeMap;
      eastl::vector_map<string_hash, string> m_NameMap;

      bool m_DirtySystems = true;

      eastl::vector_map<event_hash_name, eastl::vector<RegisteredEventQueryInfo>> m_EventHandleQueries;
      eastl::vector<RegisteredQueryInfo> m_RegisteredQueues;
      eastl::vector<DirectQuery> m_RegisteredDirectQueries;
      
      eastl::vector_map<uint64_t, EntityInfo> m_EntitiesInfo;
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
  void Registry::registerEvent()
  {
    loginfo("ecs: registering event `{}`", T::eventName);
    const string_hash hash = str_hash(T::eventName);

    ASSERT_FMT(m_EventHandleQueries.find(hash) == m_EventHandleQueries.end(),
      "ecs: can't register a new event `{}` it's already registered", T::eventName);

    m_EventHandleQueries.insert({
      hash,
      eastl::vector<RegisteredEventQueryInfo>{}
    });
  }
}
