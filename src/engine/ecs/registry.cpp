#include "registry.h"
#include "components_accessor.h"

#include <engine/log.h>
#include <engine/utils/string.h>

#include <ranges>

namespace Engine::ECS
{
  bool Registry::isTemplateRegistered(const template_name_id id) const
  {
    return m_TemplateToArhetypeMap.find(id) != m_TemplateToArhetypeMap.end();
  }

  eastl::vector<archetype_id> Registry::toArchetypeIds(const eastl::vector<string>& template_names) const
  {
    eastl::vector<archetype_id> res;
    res.reserve(template_names.size());
    for (const auto& tmpl: template_names)
    {
      const auto archId = m_TemplateToArhetypeMap.find(str_hash(tmpl.c_str()));
      res.emplace_back(archId != m_TemplateToArhetypeMap.end() ? 
                        archId->second :
                        INVALID_ARCHETYPE_ID);
    }

    return res;
  }

  void Registry::addTemplate(const string& tmpl,
                             eastl::vector<ComponentDescription>&& components,
                             const eastl::vector<string>& extends_templates)
  {
    const auto validateExtends = [&tmpl, &extends_templates](const auto& extendsArchetypes)
    {
      bool validTmpl = true;
      for (size_t i = 0; const auto& archId: extendsArchetypes)
      {
        if (archId == INVALID_ARCHETYPE_ID)
        {
          logerror("template `{}` tries to extend unknown template `{}`", tmpl, extends_templates[i]);
          validTmpl = false;
        }
        ++i;
      }
      return validTmpl;
    };

    const template_name_id templateId = str_hash(tmpl.c_str());
    if (isTemplateRegistered(templateId))
    {
      logerror("can't register a new template `{}`: it's registered already", tmpl);
      return;
    }

    const auto extendsArchetypes = toArchetypeIds(extends_templates);
    if (!validateExtends(extendsArchetypes))
      return;
    
    const archetype_id associatedArchId = m_ArchetypeConstructor(std::move(components),
                                                                 extendsArchetypes,
                                                                 extends_templates);

    m_TemplateToArhetypeMap.insert(eastl::make_pair(templateId, associatedArchId));
  }

  archetype_id Registry::getArchetypeForEntity(string_view tmpl_view)
  {
    const auto validateArchetypes = [](const auto archetypeIds, const auto& names)
    {
      bool isValid = true;
      for (size_t i = 0; const auto archId: archetypeIds)
        if (archId == INVALID_ARCHETYPE_ID)
        {
          logerror("unknown template {}", names[i]);
          isValid = false;
        }
      return isValid;
    };

    string entityTemplate{tmpl_view};
    Utils::remove_spaces(entityTemplate);
    archetype_id archetypeId = m_ArchetypeSearcher.find(entityTemplate);

    if (archetypeId != INVALID_ARCHETYPE_ID)
      return archetypeId;

    eastl::vector<string> templates = Utils::split(entityTemplate, ',');
    eastl::vector<archetype_id> archetypes = toArchetypeIds(templates);

    if (!validateArchetypes(archetypes, templates))
      return INVALID_ARCHETYPE_ID;

    archetypeId = m_ArchetypeConstructor(archetypes, templates);
    loginfo("created new archetype from [{}]", entityTemplate);

    m_DirtySystems = true;

    return archetypeId;
  }

  void Registry::createEntity(const string_view tmpl, const CreationCb& cb)
  {
    const archetype_id archetypeId = getArchetypeForEntity(tmpl);
    if (archetypeId == INVALID_ARCHETYPE_ID)
    {
      logerror("failed to create an entity");
      return;
    }

    EntityId eid = EntityId::generate();
    ChunkInfo chunkInfo = m_Archetypes[archetypeId].addEntity(eid, cb);

    const EntityInfo eInfo{
      .eid = eid,
      .archetypeId = archetypeId,
      .blockId = chunkInfo.blockId,
      .chunkId = chunkInfo.chunkId
    };

    const uint64_t id = eid.id;
    m_EntitiesInfo.insert(eastl::make_pair(id, eInfo));
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
    {
      const ArchetypeStorage& arch = m_Archetypes[i];

      bool isDesiredArchetype = true;
      for (const QueryComponentDescription& compDesc: queryComponents)
      {
        const auto it = arch.getComponentMap().find(compDesc.nameId);
        if (it == arch.getComponentMap().end() || it->second.typeId != compDesc.typeId)
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
     ComponentMap emptyCompMap;
     ComponentsAccessor compAccessor(nullptr, emptyCompMap);
     cb(event, compAccessor);
  }
}