#pragma once

#include "types.h"
#include "query.h"
#include "eid.h"
#include "components.h"
#include "archetype.h"
#include "events.h"

#include <engine/log.h>

#include <EASTL/vector_map.h>
#include <EASTL/vector.h>

namespace Engine::ECS
{
  class EntityInitializer;

  class Registry final
  {
    public:
      typedef eastl::function<void(const EntityId&, EntityInitializer&)> CreationCb;

    public:

      void addTemplate(const template_name_id& templateId, const eastl::vector<ComponentDescription>& desc);

      void createEntity(const template_name_id templateNameId, CreationCb);
      bool destroyEntity(const EntityId eid);

      void registerCppQueries();
      void registerQuery(const QueryDescription& queryDesc);

      template<class T>
      void broadcastEvent(T&& event)
      {
        if (m_EventHandleQueries.find(event.eventNameHash) != m_EventHandleQueries.end())
          m_EventsQueue.pushEvent(eastl::forward<T>(event));
        else
          logerror("can't broadcast event `{}`, it is not registered.");
      }

      inline void registerEvent(const event_hash_name event)
      {
        m_EventHandleQueries.insert({
          event,
          eastl::vector<RegisteredEventQueryInfo>{}
        });
      }

      inline static bool registerCppQuery(QueryDescription&& desc)
      {
        getCppQueryDescriptions().emplace_back(std::move(desc));
        return true;
      }

      inline static query_id registerDirectQuery(const DirectQueryDescription& queryDesc)
      {
        DirectQuery query;
        query.components = queryDesc.components;
        auto& directQueries = getDirectQueries();
        directQueries.push_back(query);

        return (query_id)(directQueries.size() - 1);
      }

      inline static eastl::vector<DirectQuery>& getDirectQueries()
      {
        static eastl::vector<DirectQuery> m_DirectQueries = eastl::vector<DirectQuery>{};
        return m_DirectQueries;
      }

      inline static eastl::vector<QueryDescription>& getCppQueryDescriptions()
      {
        static eastl::vector<QueryDescription> m_CppQueries;
        return m_CppQueries;
      }

      void query(const query_id queryId, DirectQueryCb cb);

      void tick();

    private:
      struct EntityInfo
      {
        EntityId eid;
        archetype_id archetypeId = INVALID_ARCHETYPE_ID;
        uint16_t blockId = 0;
        uint16_t chunkId = 0;
      };

    private:
      void processEvents();

      DesiredArchetypes findDesiredArchetypes(const QueryComponents& queryComponents);

      archetype_id getArchetype(const eastl::vector<ComponentDescription>& desc);

      EntityId getFreeEntity();

      template<class Cb>
      void queryArchetype(const archetype_id archetypeId, Cb cb)
      {
        Archetype& archetype = m_Archetypes[archetypeId];
        for(Chunk& chunk: archetype.m_CompStorage.m_Chunks)
          for (int i = 0; i < chunk.usedBlocks; ++i)
          {
            uint8_t* data = chunk.data + archetype.m_CompStorage.m_BlockSize * i;
            ComponentsAccessor compAccessor(data, archetype.m_ComponentsMap);
            cb(compAccessor);
          }
      }

      template<class Cb>
      void queryArchetypeByEvent(Event* event, const archetype_id archetypeId, Cb cb)
      {
        Archetype& archetype = m_Archetypes[archetypeId];
        for(Chunk& chunk: archetype.m_CompStorage.m_Chunks)
          for (int i = 0; i < chunk.usedBlocks; ++i)
          {
            uint8_t* data = chunk.data + archetype.m_CompStorage.m_BlockSize * i;
            ComponentsAccessor compAccessor(data, archetype.m_ComponentsMap);
            cb(event, compAccessor);
          }
      }

      template<class Cb>
      void processEventWithoutArchetypes(Event* event, Cb cb)
      {
         ComponentMap emptyCompMap;
         ComponentsAccessor compAccessor(nullptr, emptyCompMap);
         cb(event, compAccessor);
      }

    private:
      eastl::vector<Archetype> m_Archetypes;
      eastl::vector_map<template_name_id, archetype_id> m_TemplateToArhetypeMap;
      eastl::vector_map<event_hash_name, eastl::vector<RegisteredEventQueryInfo>> m_EventHandleQueries;

      uint64_t m_NextEntityId = 1;
      eastl::vector_map<uint64_t, EntityInfo> m_EntitiesInfo;

      eastl::vector<RegisteredQueryInfo> m_RegisteredQueues;

      EventsQueue m_EventsQueue;
  };

  void init_ecs_from_settings();
  extern Engine::ECS::Registry manager;
}
