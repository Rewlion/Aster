#pragma once

#include "types.h"
#include "query.h"
#include "eid.h"
#include "components.h"
#include "archetype.h"

#include <EASTL/vector_map.h>

class EntityInitializer;

class Registry
{
  public:
    typedef eastl::function<void(const EntityId&, EntityInitializer&)> CreationCb;

  public:

    void add_template(const template_name_id& templateId, const eastl::vector<ComponentDescription>& desc);

    void create_entity(const template_name_id templateNameId, CreationCb);
    bool destroy_entity(const EntityId eid);

    void register_cpp_queries();
    void register_query(const QueryDescription& queryDesc);

    inline static bool register_cpp_query(QueryDescription&& desc)
    {
      get_cpp_query_descriptions().emplace_back(std::move(desc));
      return true;
    }

    inline static query_id register_direct_query(const DirectQueryDescription& queryDesc)
    {
      DirectQuery query;
      query.components = queryDesc.components;
      auto& directQueries = get_direct_queries();
      directQueries.push_back(query);

      return (query_id)(directQueries.size() - 1);
    }

    inline static eastl::vector<DirectQuery>& get_direct_queries()
    {
      static eastl::vector<DirectQuery> m_DirectQueries = eastl::vector<DirectQuery>{};
      return m_DirectQueries;
    }

    inline static eastl::vector<QueryDescription>& get_cpp_query_descriptions()
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
    DesiredArchetypes find_desired_archetypes(const QueryComponents& queryComponents);

    archetype_id get_archetype(const eastl::vector<ComponentDescription>& desc);

    EntityId get_free_entity();

    template<class Cb>
    void query_archetype(const archetype_id archetypeId, Cb cb)
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

  private:
    eastl::fixed_vector<Archetype, 128, true> m_Archetypes;
    eastl::vector_map<template_name_id, archetype_id> m_TemplateToArhetypeMap;

    uint64_t m_NextEntityId = 1;
    eastl::vector_map<uint64_t, EntityInfo> m_EntitiesInfo;

    eastl::fixed_vector<RegisteredQueryInfo, 128, true> m_RegisteredQueues;
};

void init_ecs_from_settings();

extern Registry ecs;