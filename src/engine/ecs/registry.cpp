#include "registry.h"

#include "components_accessor.h"

#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>
#include <engine/log.h>

Registry ecs;

archetype_id Registry::get_archetype(const eastl::vector<ComponentDescription>& desc)
{
  for (size_t i = 0; i < m_Archetypes.size(); ++i)
    if (m_Archetypes[i].has_components(desc))
      return (archetype_id)i;

  return INVALID_ARCHETYPE_ID;
}

void Registry::add_template(const template_name_id& templateId, const eastl::vector<ComponentDescription>& desc)
{
  archetype_id archetypeId = get_archetype(desc);
  if (archetypeId == INVALID_ARCHETYPE_ID)
  {
    archetypeId = (archetype_id)m_Archetypes.size();
    m_Archetypes.emplace_back(desc);
  }
  m_TemplateToArhetypeMap.insert({templateId, archetypeId});
}

void Registry::create_entity(const template_name_id templateNameId, CreationCb cb)
{
  const auto it = m_TemplateToArhetypeMap.find(templateNameId);
  ASSERT(it != m_TemplateToArhetypeMap.end());
  archetype_id archetypeId = it->second;
  EntityId eid = get_free_entity();

  Archetype& archetype = m_Archetypes[archetypeId];
  EntityInitializer init = archetype.get_new_entity_initializer();
  cb(eid, init);

  uint16_t blockId = 0, chunkId = 0;

  archetype.m_CompStorage.add_entity(init.m_Data, init.m_ComponentMap.size(), init.m_Offsets, init.m_Sizes, chunkId, blockId);

  const EntityInfo eInfo{
    .eid = eid,
    .archetypeId = archetypeId,
    .blockId = blockId,
    .chunkId = chunkId
  };

  m_EntitiesInfo.insert({
    eid.id,
    eInfo
  });
}

bool Registry::destroy_entity(const EntityId eid)
{
  auto it = m_EntitiesInfo.find(eid.id);
  if (it == m_EntitiesInfo.end())
    return false;

  EntityInfo& eInfo = it->second;
  if (eInfo.eid.generation != eid.generation)
    return false;

  Archetype& archetype = m_Archetypes[eInfo.archetypeId];

  block_id replacedBlock = INVALID_BLOCK_ID;
  archetype.m_CompStorage.destroy_entity(archetype.m_ComponentsMap, eInfo.chunkId, eInfo.blockId, replacedBlock);

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

DesiredArchetypes Registry::find_desired_archetypes(const QueryComponents& queryComponents)
{
  DesiredArchetypes desiredArchetypes;

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

void Registry::register_cpp_queries()
{
  const auto& cppQueries = get_cpp_query_descriptions();
  for(const QueryDescription& desc: cppQueries)
    register_query(desc);

  for(DirectQuery& query: get_direct_queries())
    query.desiredArchetypes = find_desired_archetypes(query.components);
}

void Registry::register_query(const QueryDescription& queryDesc)
{
  DesiredArchetypes desiredArchetypes = find_desired_archetypes(queryDesc.components);

  if (queryDesc.event != INVALID_HASH && desiredArchetypes.size() > 0)
  {
    auto it = m_EventHandleQueries.find(queryDesc.event);
    if (it != m_EventHandleQueries.end())
    {
      it->second.emplace_back(queryDesc.eventCb, eastl::move(desiredArchetypes));
    }
  }
  else
  {
    if (desiredArchetypes.size() > 0)
      m_RegisteredQueues.emplace_back(queryDesc.cb, desiredArchetypes);
  }
}

void Registry::query(const query_id queryId, DirectQueryCb cb)
{
  DirectQuery& directQuery = get_direct_queries()[queryId];
  for (archetype_id archetypeId: directQuery.desiredArchetypes)
    query_archetype(archetypeId, cb);
}

EntityId Registry::get_free_entity()
{
  EntityId eid(m_NextEntityId++);
  return eid;
}

void Registry::tick()
{
  process_events();

  for(const RegisteredQueryInfo& query: m_RegisteredQueues)
    for(const archetype_id archetypeId: query.archetypes)
      query_archetype(archetypeId, query.cb);
}

void Registry::process_events()
{
  Event* event = m_EventsQueue.pop_event();
  while (event != nullptr)
  {
    eastl::vector<RegisteredEventQueryInfo>& queries = m_EventHandleQueries[event->eventNameHash];
    for(const auto& query: queries)
      for(const archetype_id archetypeId: query.archetypes)
        query_archetype_by_event(event, archetypeId, query.eventCb);

    event->_destr(event);
  }
}

void init_ecs_from_settings()
{
  log("initialization of ECS");
  DataBlock* settings = get_app_settings();
  DataBlock* templates = settings->get_child_block("entity_templates");
  for(const auto& attr: templates->get_attributes())
  {
    if (attr.type == DataBlock::ValueType::Text)
    {
      const String blkWithTemplates = std::get<String>(attr.as);
      log("reading templates from {}", blkWithTemplates);

      add_templates_from_blk(ecs, blkWithTemplates);
    }
  }

  ecs.register_cpp_queries();
}