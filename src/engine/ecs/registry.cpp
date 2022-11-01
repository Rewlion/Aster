#include "registry.h"

#include "components_accessor.h"

#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>
#include <engine/log.h>

namespace Engine::ECS
{
  Engine::ECS::Registry manager;

  archetype_id Registry::getArchetype(const eastl::vector<ComponentDescription>& desc)
  {
    for (size_t i = 0; i < m_Archetypes.size(); ++i)
      if (m_Archetypes[i].hasComponents(desc))
        return (archetype_id)i;

    return INVALID_ARCHETYPE_ID;
  }

  void Registry::addTemplate(const template_name_id& templateId, const eastl::vector<ComponentDescription>& desc)
  {
    archetype_id archetypeId = getArchetype(desc);
    if (archetypeId == INVALID_ARCHETYPE_ID)
    {
      archetypeId = (archetype_id)m_Archetypes.size();
      m_Archetypes.emplace_back(desc);
    }
    m_TemplateToArhetypeMap.insert({templateId, archetypeId});
  }

  void Registry::createEntity(const template_name_id templateNameId, CreationCb cb)
  {
    const auto it = m_TemplateToArhetypeMap.find(templateNameId);
    ASSERT(it != m_TemplateToArhetypeMap.end());
    archetype_id archetypeId = it->second;
    EntityId eid = getFreeEntity();

    Archetype& archetype = m_Archetypes[archetypeId];
    EntityInitializer init = archetype.getNewEntityInitializer();
    cb(eid, init);

    uint16_t blockId = 0, chunkId = 0;

    archetype.m_CompStorage.addEntity(init.m_Data, init.m_ComponentMap.size(), init.m_Offsets, init.m_Sizes, chunkId, blockId);

    const EntityInfo eInfo{
      .eid = eid,
      .archetypeId = archetypeId,
      .blockId = blockId,
      .chunkId = chunkId
    };

    const uint64_t id = eid.id;
    m_EntitiesInfo.insert({
      id,
      eInfo
    });
  }

  bool Registry::destroyEntity(const EntityId eid)
  {
    auto it = m_EntitiesInfo.find(eid.id);
    if (it == m_EntitiesInfo.end())
      return false;

    EntityInfo& eInfo = it->second;
    if (eInfo.eid.generation != eid.generation)
      return false;

    Archetype& archetype = m_Archetypes[eInfo.archetypeId];

    block_id replacedBlock = INVALID_BLOCK_ID;
    archetype.m_CompStorage.destroyEntity(archetype.m_ComponentsMap, eInfo.chunkId, eInfo.blockId, replacedBlock);

    eInfo.eid.generation += 1;

    if (replacedBlock != INVALID_BLOCK_ID)
      for (auto& [_, replacingEInfo]: m_EntitiesInfo)
        if (replacingEInfo.blockId == replacedBlock)
        {
          replacingEInfo.blockId = eInfo.blockId;
          break;
        }

    return true;
  }

  DesiredArchetypes Registry::findDesiredArchetypes(const QueryComponents& queryComponents)
  {
    DesiredArchetypes desiredArchetypes;
    if (queryComponents.empty())
      return desiredArchetypes;

    for (size_t i = 0; i < m_Archetypes.size(); ++i)
    {
      const Archetype& archetype = m_Archetypes[i];

      bool isDesiredArchetype = true;
      for (const QueryComponentDescription& compDesc: queryComponents)
      {
        const auto it = archetype.m_ComponentsMap.find(compDesc.nameId);
        if (it == archetype.m_ComponentsMap.end() || it->second.typeId != compDesc.typeId)
        {
          isDesiredArchetype = false;
          break;
        }
      }
      if (isDesiredArchetype)
      {
        desiredArchetypes.emplace_back(i);
      }
    }

    return desiredArchetypes;
  }

  void Registry::registerCppQueries()
  {
    SystemRegistration::enumerate([this](const SystemRegistration& system_reg) {
      registerSystem(system_reg.m_Cb, system_reg.m_Comps);
    });

    EventSystemRegistration::enumerate([this](const EventSystemRegistration& event_system_reg) {
      registerEventSystem(event_system_reg.m_Cb, event_system_reg.m_Event, event_system_reg.m_Comps);
    });

    for(DirectQuery& query: getDirectQueries())
      query.desiredArchetypes = findDesiredArchetypes(query.components);
  }

  void Registry::registerSystem(const QueryCb& cb, const QueryComponents& components)
  {
    const DesiredArchetypes desiredArchetypes = findDesiredArchetypes(components);
    if (desiredArchetypes.size() > 0)
    {
      m_RegisteredQueues.push_back(RegisteredQueryInfo{
        .cb = std::move(cb),
        .archetypes = std::move(desiredArchetypes)
      });
    }
  }

  void Registry::registerEventSystem(const EventQueryCb& cb, const event_hash_name event, const QueryComponents& components)
  {
    if (event != INVALID_HASH)
    {
      const DesiredArchetypes desiredArchetypes = findDesiredArchetypes(components);

      auto it = m_EventHandleQueries.find(event);
      if (it != m_EventHandleQueries.end())
      {
        it->second.push_back(RegisteredEventQueryInfo{
          .eventCb = cb,
          .archetypes = eastl::move(desiredArchetypes)
        });
      }
    }
  }

  void Registry::query(const query_id queryId, DirectQueryCb cb)
  {
    DirectQuery& directQuery = getDirectQueries()[queryId];
    for (archetype_id archetypeId: directQuery.desiredArchetypes)
      queryArchetype(archetypeId, cb);
  }

  EntityId Registry::getFreeEntity()
  {
    EntityId eid(m_NextEntityId++);
    return eid;
  }

  void Registry::tick()
  {
    processEvents();

    for(const RegisteredQueryInfo& query: m_RegisteredQueues)
      for(const archetype_id archetypeId: query.archetypes)
        queryArchetype(archetypeId, query.cb);
  }

  void Registry::processEvents()
  {
    while (Event* event = m_EventsQueue.popEvent())
    {
      eastl::vector<RegisteredEventQueryInfo>& queries = m_EventHandleQueries[event->eventNameHash];

      for(const auto& query: queries)
      {
        if (!query.archetypes.empty())
        {
          for(const archetype_id archetypeId: query.archetypes)
            queryArchetypeByEvent(event, archetypeId, query.eventCb);
        }
        else
          processEventWithoutArchetypes(event, query.eventCb);
      }

      event->_destr(event);
    }
  }

  void init_ecs_from_settings()
  {
    loginfo("initialization of ECS");
    DataBlock* settings = get_app_settings();

    loginfo("init events");
    DataBlock* events = settings->getChildBlock("events");
    for(const auto& attr: events->getAttributes())
    {
      if (attr.name == "event")
      {
        const string eventName = std::get<string>(attr.as);
        manager.registerEvent(str_hash(eventName.c_str()));
        loginfo("registered event `{}`", eventName);
      }
    }

    loginfo("init templates");
    DataBlock* templates = settings->getChildBlock("entity_templates");
    for(const auto& attr: templates->getAttributes())
    {
      if (attr.type == DataBlock::ValueType::Text)
      {
        const string blkWithTemplates = std::get<string>(attr.as);
        loginfo("reading templates from {}", blkWithTemplates);

        add_templates_from_blk(manager, blkWithTemplates);
      }
    }

    manager.registerCppQueries();
  }
}