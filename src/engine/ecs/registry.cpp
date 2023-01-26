#include "registry.h"
#include "components_accessor.h"

#include <engine/ecs/fs/load_templates.h>
#include <engine/settings.h>
#include <engine/log.h>

#include <ranges>

namespace Engine::ECS
{
  Engine::ECS::Registry manager;

  struct ArchetypeExtensionInfo
  {
    const string tmplName;
    template_name_id templateId;
    archetype_id archetypeId;
  };

  static string get_templates_final_name(const eastl::vector<string>& templates)
  {
    string res = templates[0];
    for (size_t i = 1; i < templates.size(); ++i)
      res += ',' + templates[i];

    return res;
  }

  archetype_id Registry::getArchetype(const eastl::vector<string>& templates)
  {
    struct TemplateNameAndId
    {
      const string& name;
      const archetype_id id;
    };

    const auto to_template_name_and_arch_id = [this]() {
      return std::views::transform([this](const string& tmpl_name) {
        const auto it = m_TemplateToArhetypeMap.find(str_hash(tmpl_name.c_str()));
        return TemplateNameAndId{ tmpl_name, it != m_TemplateToArhetypeMap.end() ? it->second : INVALID_ARCHETYPE_ID };
      });
    };

    const string tmpl = get_templates_final_name(templates);
    const template_name_id requestedTmplId = str_hash(tmpl.c_str());
    const auto currentArchIt = m_TemplateToArhetypeMap.find(requestedTmplId);

    if (currentArchIt != m_TemplateToArhetypeMap.end())
      return currentArchIt->second;

    ArchetypeComponentsDescription newArchDesc;
    for (const auto [name, archId] : templates | to_template_name_and_arch_id())
    {
      if (archId == INVALID_ARCHETYPE_ID)
      {
        logerror("invalid template `{}`: it's not registered", name);
        return INVALID_ARCHETYPE_ID;
      }

      newArchDesc.merge(m_Archetypes[archId].getDescription());
    }

    const archetype_id newArchId = m_Archetypes.size();
    m_Archetypes.emplace_back(std::move(newArchDesc));
    m_TemplateToArhetypeMap.emplace(eastl::make_pair(requestedTmplId, newArchId));

    for (const auto [_, oldArchId] : templates | to_template_name_and_arch_id())
    {
      const auto it = m_PendingArchetypeRegistrations.find(oldArchId);
      if (it != m_PendingArchetypeRegistrations.end())
      {
        it->second.emplace_back(newArchId);
      }
      else
      {
        m_PendingArchetypeRegistrations.emplace(
          eastl::make_pair(oldArchId, eastl::vector{newArchId})
        );
      }
    }

    loginfo("created new archetype from templates: {}", tmpl);
    return newArchId;
  }

  archetype_id Registry::getArchetype(const ArchetypeComponentsDescription& desc)
  {
    for (size_t i = 0; i < m_Archetypes.size(); ++i)
      if (m_Archetypes[i].hasComponents(desc.components))
        return (archetype_id)i;

    return INVALID_ARCHETYPE_ID;
  }

  template<class T>
  inline decltype(auto) transform_to_extension_info(const T& template_to_archetype_id_map)
  {
    return std::views::transform([&template_to_archetype_id_map](const string& str){
      const template_name_id tmplId = str_hash(str.c_str());
      const auto archIt = template_to_archetype_id_map.find(tmplId);
      return ArchetypeExtensionInfo{
        .tmplName = str,
        .templateId = tmplId,
        .archetypeId = archIt != template_to_archetype_id_map.end() ? archIt->second : INVALID_ARCHETYPE_ID
        };
    });
  }

  static inline decltype(auto) filter_invalid_archetypes()
  {
    return std::views::filter(
      [](const ArchetypeExtensionInfo& ext)
        { return ext.archetypeId == INVALID_ARCHETYPE_ID; });
  }

  void Registry::addTemplate(const string& tmpl,
                             eastl::vector<ComponentDescription>&& desc,
                             const eastl::vector<string>& extends_templates)
  {
    const template_name_id templateId = str_hash(tmpl.c_str());
    if (m_TemplateToArhetypeMap.find(templateId) != m_TemplateToArhetypeMap.end())
    {
      logerror("template {} is registered already", tmpl);
      return;
    }

    auto extensions = extends_templates | transform_to_extension_info(m_TemplateToArhetypeMap);
    auto invalidExtensions = extensions | filter_invalid_archetypes();
    auto invalidExtensionsCount = std::distance(invalidExtensions.begin(), invalidExtensions.end());
    if (invalidExtensionsCount != 0)
    {
      logerror("template `{}` tries to extend unknown templates", tmpl);
      for (const auto& ext: invalidExtensions)
        logerror("invalid extend: {}", ext.tmplName);
      return;
    }

    ArchetypeComponentsDescription archDesc{std::move(desc)};
    for (const auto& parentExt: extensions)
    {
      if (!archDesc.merge(m_Archetypes[parentExt.archetypeId].getDescription()))
      {
        logerror("template `{}` is invalid. Failed to extend `{}` see previous errors", tmpl, parentExt.tmplName);
        return;
      }
    }

    archetype_id archetypeId = getArchetype(archDesc);
    if (archetypeId == INVALID_ARCHETYPE_ID)
    {
      archetypeId = (archetype_id)m_Archetypes.size();
      m_Archetypes.emplace_back(std::move(archDesc));
    }
    m_TemplateToArhetypeMap.insert({templateId, archetypeId});
  }

  void Registry::createEntity(const eastl::vector<string>& templates, CreationCb cb)
  {
    const archetype_id archetypeId = getArchetype(templates);
    if (archetypeId == INVALID_ARCHETYPE_ID)
    {
      logerror("failed to create an entity");
      return;
    }

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
    processPendingArchetypeRegistrations();
    processEvents();

    for(const RegisteredQueryInfo& query: m_RegisteredQueues)
      for(const archetype_id archetypeId: query.archetypes)
        queryArchetype(archetypeId, query.cb);
  }

  void Registry::processPendingArchetypeRegistrations()
  {
    for (const auto [oldArchId, newArchIds] : m_PendingArchetypeRegistrations)
    {
      for (auto& registeredQuery: m_RegisteredQueues)
      {
        const bool queryShouldHandleNewArchetype =
          registeredQuery.archetypes.end() != 
          eastl::find(registeredQuery.archetypes.begin(), registeredQuery.archetypes.end(), oldArchId);
        
        if (queryShouldHandleNewArchetype)
          for (auto& newArchId: newArchIds)
            registeredQuery.archetypes.emplace_back(newArchId);
      }

      for (auto& [event, queries]: m_EventHandleQueries)
      {
        for (auto& query: queries)
        {
          const bool queryShouldHandleNewArchetype =
            query.archetypes.end() != 
            eastl::find(query.archetypes.begin(), query.archetypes.end(), oldArchId);
        
          if (queryShouldHandleNewArchetype)
            for (auto& newArchId: newArchIds)
              query.archetypes.emplace_back(newArchId);
        }
      }
    }
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