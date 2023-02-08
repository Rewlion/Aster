#include "registry.h"
#include "components.h"
#include "components_accessor.h"

#include <engine/log.h>
#include <engine/utils/string.h>

#include <ranges>

namespace Engine::ECS
{
  bool Registry::isTemplateRegistered(const template_name_id id) const
  {
    return m_TemplateToArchetypeMap.find(id) != m_TemplateToArchetypeMap.end();
  }

  eastl::vector<archetype_id> Registry::toArchetypeIds(const eastl::vector<string>& template_names) const
  {
    eastl::vector<archetype_id> res;
    res.reserve(template_names.size());

    bool isValid = true;
    for (const auto& tmpl: template_names)
    {
      const auto& tmplIt = m_TemplateToArchetypeMap.find(str_hash(tmpl.c_str()));
      if (tmplIt != m_TemplateToArchetypeMap.end())
        res.push_back(tmplIt->second);
      else
      {
        logerror("ecs: unknown template {}", tmpl);
        isValid = false;
      }
    }

    return isValid ? res : eastl::vector<archetype_id>{};
  }

  static eastl::vector<ArchetypeComponent> to_arch_comps(const eastl::vector<TemplateComponentDescription>& components)
  {
    eastl::vector<ArchetypeComponent> res;
    res.reserve(components.size());

    for (auto& c: components)
    {
      res.push_back({
        .nameId = str_hash(c.name.c_str()),
        .typeId = get_meta_storage().getMeta(c.type).typeId,
        .blockOffset = 0
      });
    }
    
    return res;
  }

  eastl::tuple<CompToPlacementMap,size_t>
   Registry::placeArchetypeTypes(eastl::vector<ArchetypeComponent>& types) const
  {
    CompToPlacementMap compToPlacements;
    size_t blockOffset = 0;

    for(size_t i = 0; auto& type: types)
    {
      type.blockOffset = blockOffset;
      compToPlacements.insert({type.nameId, i});

      blockOffset += get_meta_storage().getMeta(type.typeId).size;;
      Sys::align(blockOffset);
      ++i;
    }
  
    return {compToPlacements, blockOffset};
  }

  bool Registry::mergeArchetypeComponents(eastl::vector<ArchetypeComponent>& to, 
                                          const eastl::vector<ArchetypeComponent>& from_comps) const
  {
    bool isValid = true;
    for (const auto& from: from_comps)
    {
      const auto toIt = eastl::find_if(
        to.begin(),
        to.end(),
        [&from](const ArchetypeComponent& t) { return t.nameId == from.nameId; }
      );

      if (toIt == to.end())
        to.push_back(from);
      else
      {
        if (toIt->typeId != from.typeId)
        {
          const string& compName = m_NameMap.find(toIt->nameId)->second;
          const char* toTypeName = get_meta_storage().getMeta(toIt->typeId).typeName;
          const char* fromTypeName = get_meta_storage().getMeta(from.typeId).typeName;
          logerror("ecs: failed to merge component `{}`: invalid types {} != {}",
            compName, toTypeName, fromTypeName);
          isValid = false;
        }
      }
    }
    return isValid;
  }                               

  archetype_id Registry::makeArchetype(eastl::vector<ArchetypeComponent>&& comps)
  {
    auto [compsMap, blockSize] = placeArchetypeTypes(comps);

    const archetype_id archetypeId = (archetype_id)m_Archetypes.size();
    m_Archetypes.emplace_back(blockSize, std::move(comps), std::move(compsMap));

    return archetypeId;
  }

  archetype_id Registry::findArchetype(const eastl::vector<ArchetypeComponent>& comps) const
  {
    for (size_t i = 0; i < m_Archetypes.size(); ++i)
     if (m_Archetypes[i].hasComponents(comps))
       return (archetype_id)i;

    return INVALID_ARCHETYPE_ID;
  }

  archetype_id Registry::makeArchetype(eastl::vector<ArchetypeComponent>&& comps,
                                       const eastl::vector<archetype_id>& extends)
  {
    for (const archetype_id archId: extends)
      if (!mergeArchetypeComponents(comps, m_Archetypes[archId].getComponents()))
        return INVALID_ARCHETYPE_ID;

    archetype_id archetypeId = findArchetype(comps);
    if (archetypeId != INVALID_ARCHETYPE_ID)
      return archetypeId;

    return makeArchetype(std::move(comps));
  }

  void Registry::addTemplate(const string& tmpl,
                             eastl::vector<TemplateComponentDescription>&& components,
                             const eastl::vector<string>& extends_templates)
  {
    const string tmplName = Utils::remove_spaces(tmpl);
    const template_name_id templateId = str_hash(tmplName.c_str());

    if (isTemplateRegistered(templateId))
    {
      logerror("can't register a new template `{}`: it's registered already", tmplName);
      return;
    }

    const auto extendsArchetypes = toArchetypeIds(extends_templates);
    if (!extends_templates.empty() && extendsArchetypes.empty())
    {
      logerror("ecs: failed to register template `{}`: there are invalid extensions", tmpl);
      return;
    }
    
    eastl::vector<ArchetypeComponent> comps = to_arch_comps(components);
    for (size_t i = 0; const auto& comp: comps)
    {
      m_NameMap.insert({comp.nameId, std::move(components[i].name)});
      ++i;
    }

    const archetype_id archId = makeArchetype(std::move(comps), extendsArchetypes);
    if (archId != INVALID_ARCHETYPE_ID)
      m_TemplateToArchetypeMap.insert({templateId, archId});
    else
      logerror("ecs: failed to register template `{}`: there are invalid extensions", tmpl);
  }

  archetype_id Registry::mergeTemplates(const string final_name,
                                        const template_name_id final_id,
                                        const eastl::vector<string>& tmpl_names)
  {
    eastl::vector<archetype_id> archetypes = toArchetypeIds(tmpl_names);
    eastl::vector<ArchetypeComponent> components;
    
    bool isValid = true;
    for (size_t i = 0; auto archId: archetypes)
    {
      if (!mergeArchetypeComponents(components, m_Archetypes[archId].getComponents()))
      {
        logerror("ecs: failed to merge template {}", tmpl_names[i]);
        isValid = false;
      }
      ++i;
    }

    if (isValid)
    {
      loginfo("ecs: merged templates `{}`", final_name);
      return makeArchetype(std::move(components));
    }
    else
      return INVALID_ARCHETYPE_ID;
  }

  EntityComponents Registry::makeEntityComponents(const archetype_id arch_id) const
  {
    const ArchetypeStorage& archetype = m_Archetypes[arch_id];
    return EntityComponents{
      archetype.getBlockSize(),
      archetype.getComponents(),
      archetype.getCompToPlacementsMap()
    };
  }

  void Registry::createEntity(const string_view tmpl, const CreationCb& cb)
  {
    const string tmplNameList = Utils::remove_spaces(string{tmpl});
    eastl::vector<string> templateNames = Utils::split(tmplNameList, ',');

    ASSERT_FMT(!templateNames.empty(), "ecs: can't create entity from empty template");

    archetype_id archetypeId = INVALID_ARCHETYPE_ID;
    if (templateNames.size() == 1)
    {
      const auto name = templateNames[0];
      const template_name_id tmplId = str_hash(name.c_str());
      const auto it = m_TemplateToArchetypeMap.find(tmplId);

      if (it != m_TemplateToArchetypeMap.end())
        archetypeId = it->second;
      else
        logerror("ecs: unknown template {}", name);
    }
    else
    {
      const template_name_id tmplId = str_hash(tmplNameList.c_str());
      const auto it = m_TemplateToArchetypeMap.find(tmplId);
      archetypeId = it != m_TemplateToArchetypeMap.end() ?
                      it->second :
                      mergeTemplates(tmplNameList, tmplId, templateNames);
      m_DirtySystems = true;
    }

    if (archetypeId == INVALID_ARCHETYPE_ID)
    {
      logerror("ecs: failed to create entity from `{}`", tmplNameList);
      return;
    }
    
    EntityId eid = EntityId::generate();
    EntityComponents init = makeEntityComponents(archetypeId);

    cb(eid, init);

    ChunkInfo chunkInfo = m_Archetypes[archetypeId].addEntity(eid, std::move(init));

    const EntityInfo eInfo{
      .eid = eid,
      .archetypeId = archetypeId,
      .blockId = chunkInfo.blockId,
      .chunkId = chunkInfo.chunkId
    };

    const uint64_t id = eid.id;
    m_EntitiesInfo.insert(eastl::make_pair(id, eInfo));

    loginfo("ecs: created entity[{}:{}] from `{}`",
      eid.getId(), eid.getGeneration(), tmplNameList);
  }

  bool Registry::destroyEntity(const EntityId eid)
  {
    auto it = m_EntitiesInfo.find(eid.id);
    if (it == m_EntitiesInfo.end())
      return false;

    EntityInfo& eInfo = it->second;
    if (eInfo.eid.generation != eid.generation)
      return false;

    ArchetypeStorage& archetype = m_Archetypes[eInfo.archetypeId];
    DestroidEntityInfo dstrInfo = archetype.destroyEntity(eInfo.chunkId, eInfo.blockId);

    eInfo.eid.generation += 1;

    if (dstrInfo.replacedBlockId != INVALID_BLOCK_ID)
    {
      EntityInfo eInfoToTweak = m_EntitiesInfo[dstrInfo.replacedBlockOwner.getId()];
      eInfoToTweak.blockId = dstrInfo.replacedBlockId;
    }

    return true;
  }

  DesiredArchetypes Registry::findDesiredArchetypes(const QueryComponents& queryComponents)
  {
    DesiredArchetypes desiredArchetypes;
    if (queryComponents.empty())
      return desiredArchetypes;

    for (size_t i = 0; i < m_Archetypes.size(); ++i)
      if (m_Archetypes[i].hasComponents(queryComponents))
        desiredArchetypes.push_back(i);

    return desiredArchetypes;
  }

  void Registry::registerCppQueries()
  {
    m_RegisteredQueues.clear();
    for (auto& [event, queries] : m_EventHandleQueries)
      queries.clear();
    m_RegisteredDirectQueries.clear();
    m_RegisteredDirectQueries.resize(DirectQueryRegistration::size());

    SystemRegistration::enumerate([this](const SystemRegistration& system_reg) {
      registerSystem(system_reg.m_Cb, system_reg.m_Comps);
    });

    DirectQueryRegistration::enumerate([this](const DirectQueryRegistration& query) {
      m_RegisteredDirectQueries[query.getId()].desiredArchetypes = 
        findDesiredArchetypes(query.getComponents());
    });

    EventSystemRegistration::enumerate([this](const EventSystemRegistration& event_system_reg) {
      registerEventSystem(event_system_reg.m_Cb, event_system_reg.m_Event, event_system_reg.m_Comps);
    });
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
    DirectQuery& directQuery = m_RegisteredDirectQueries[queryId];
    for (archetype_id archetypeId: directQuery.desiredArchetypes)
      queryArchetype(archetypeId, cb);
  }

  void Registry::tick()
  {
    if (m_DirtySystems)
    {
      registerCppQueries();
      m_DirtySystems = false;
    }

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

  void Registry::queryArchetype(const archetype_id archetypeId, QueryCb cb)
  {
    ArchetypeStorage& archetype = m_Archetypes[archetypeId];
    archetype.forEachEntity([cb](ComponentsAccessor& comps)
    {
      cb(comps);
    });
  }

  void Registry::queryArchetype(const archetype_id archetypeId, const DirectQueryCb& cb)
  {
    ArchetypeStorage& archetype = m_Archetypes[archetypeId];
    archetype.forEachEntity([&cb](ComponentsAccessor& comps)
    {
      cb(comps);
    });
  }

  void Registry::queryArchetypeByEvent(Event* event, const archetype_id archetypeId, EventQueryCb cb)
  {
    ArchetypeStorage& archetype = m_Archetypes[archetypeId];
    archetype.forEachEntity([cb, event](ComponentsAccessor& comps)
    {
      cb(event, comps);
    });
  }

  void Registry::processEventWithoutArchetypes(Event* event, EventQueryCb cb)
  {
     CompToPlacementMap emptyCompMap;
     const eastl::vector<ArchetypeComponent> emptyTypes;
     ComponentsAccessor compAccessor(nullptr, emptyCompMap, emptyTypes);
     cb(event, compAccessor);
  }
}