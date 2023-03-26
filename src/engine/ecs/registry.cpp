#include "registry.h"
#include "components_accessor.h"

#include <engine/log.h>
#include <engine/utils/string.h>

#include <ranges>

namespace ecs
{
  void Registry::addTemplate(const string& name,
                            TemplateParentNames&& parents,
                            TemplateComponentsMap&& comps_map)
  {
    if (m_Templates.hasTemplateDescription(name)) [[unlikely]]
    {
      logerror("ecs: failed to register template `{}`: it's already registered", name);
      return;
    }

    for (const auto& parent: parents)
    {
      if (!m_Templates.hasTemplateDescription(parent))
      {
        logerror("ecs: failed to register template `{}`: parent `{}` is unknown", name, parent);
        return;
      }
    }

    const bool added = m_Templates.addTemplateDesc(TemplateDescription{name, std::move(parents), std::move(comps_map)},
                                                   m_RegisteredComponents);
    if (!added) [[unlikely]]
      logerror("ecs: failed to register template `{}`, see logs above", name);
  }

  auto Registry::createEntity(const string_view tmpl_name, EntityComponents&& inits) -> EntityId
  {
    const template_id_t tmplId = m_Templates.requestTemplate(tmpl_name, m_Archetypes, m_RegisteredComponents);
    if (tmplId == INVALID_TEMPLATE_ID) [[unlikely]]
    {
      logerror("ecs: failed to create entity from `{}`", tmpl_name);
      return {};
    }

    const Template& tmpl = m_Templates.getTemplate(tmplId);
    const archetype_id_t archId = tmpl.archetypeId;

    const auto& offsets = m_Archetypes.getComponentOffsets(archId);
    const auto& types = m_Archetypes.getComponentTypes(archId);
    const auto& sizes = m_Archetypes.getComponentSizes(archId);
    const auto& compIds = m_Archetypes.getComponentIds(archId);
    const components_count_t compsCount = m_Archetypes.getComponentsCount(archId);

    eastl::vector_map<registered_component_id_t, const TemplateComponent*> compIdToInit;
    for (const EntityComponentInit& init: inits.getInits())
    {
      const registered_component_id_t compId = m_RegisteredComponents.getComponentId(init.name.hash);
      if (compId == INVALID_REG_COMPONENT_ID) [[unlikely]]
      {
        logerror("ecs: unknown component ({} {}) during entity construction with tmpl {}, skipping this component",
          init.component.getMeta()->typeName, init.name.str, tmpl_name);
        continue;
      }

      const bool tmplHasComponent = tmpl.regCompToTmplComp.find(compId) != tmpl.regCompToTmplComp.end();
      if (!tmplHasComponent) [[unlikely]]
      {
        logerror("ecs: template {} doesn't have component ({} {}), skipping this component",
          tmpl_name, init.component.getMeta()->typeName, init.name.str);
        continue;
      }

      compIdToInit.insert({compId, &init.component});
    }

    std::byte* initBuffer = tmpl.initBuffer.get();

    for (components_count_t i{0}; i < compsCount; ++i)
    {
      std::byte* compDataForInit = initBuffer + offsets[i];
      const registered_component_id_t compId = compIds[i];
      
      const TemplateComponent* tmplComp = nullptr;
      if (auto initIt = compIdToInit.find(compId); initIt != compIdToInit.end())
        tmplComp = initIt->second;
      else
        tmplComp = tmpl.regCompToTmplComp.find(compIds[i])->second;

      const void* tmplInitData = tmplComp->getData();

      get_meta_storage()
        .getMeta(types[i])
        ->manager->constructor(compDataForInit, tmplInitData);
    }

    EntityStorage& storage = m_Archetypes.getEntityStorage(archId);
    const EntityId newEid = EntityId::generate();
    const auto [chunkId, chunkEid] = storage.allocateEntityPlace();
    storage.placeEntity(compsCount,
                        initBuffer,
                        offsets.data(),
                        sizes.data(),
                        newEid,
                        chunkId, chunkEid);

    m_EntityInfosMap.insert(eastl::make_pair(
      newEid.getId(), 
      EntityInfo{
        .eid = newEid,
        .chunkId = chunkId,
        .chunkEid = chunkEid
      }
    ));

    return newEid;
  }

  bool Registry::destroyEntity(const EntityId eid)
  {
    // auto it = m_EntitiesInfo.find(eid.id);
    // if (it == m_EntitiesInfo.end())
    //   return false;

    // EntityInfo& eInfo = it->second;
    // if (eInfo.eid.generation != eid.generation)
    //   return false;

    // ArchetypeStorage& archetype = m_Archetypes[eInfo.archetypeId];
    // DestroidEntityInfo dstrInfo = archetype.destroyEntity(eInfo.chunkId, eInfo.blockId);

    // eInfo.eid.generation += 1;

    // if (dstrInfo.replacedBlockId != INVALID_BLOCK_ID)
    // {
    //   EntityInfo eInfoToTweak = m_EntitiesInfo[dstrInfo.replacedBlockOwner.getId()];
    //   eInfoToTweak.blockId = dstrInfo.replacedBlockId;
    // }

    return true;
  }

  auto Registry::findDesiredArchetypes(const QueryComponents& queryComponents)
    -> eastl::vector<archetype_id_t>
  {
    eastl::vector<registered_component_id_t> comps;
    comps.reserve(queryComponents.size());
    for (const auto& qc: queryComponents)
    {
      const auto [rc, compId] = m_RegisteredComponents.getComponentAndId(qc.nameHash);
      if (rc->typeMeta.typeId != qc.typeId)
        return {};
      
      comps.push_back(compId);
    }

    return m_Archetypes.findArchetypesWithComponents(comps);
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
    const  eastl::vector<archetype_id_t> desiredArchetypes = findDesiredArchetypes(components);
    if (desiredArchetypes.size() > 0)
    {
      m_RegisteredQueues.push_back(RegisteredQueryInfo{
        .cb = std::move(cb),
        .archetypes = std::move(desiredArchetypes)
      });
    }
  }

  void Registry::registerEventSystem(const EventQueryCb& cb,
                                     const name_hash_t event,
                                     const QueryComponents& components)
  {
    const  eastl::vector<archetype_id_t> desiredArchetypes = findDesiredArchetypes(components);

    auto it = m_EventHandleQueries.find(event);
    if (it != m_EventHandleQueries.end())
    {
      it->second.push_back(RegisteredEventQueryInfo{
        .eventCb = cb,
        .archetypes = eastl::move(desiredArchetypes)
      });
    }
  }

  void Registry::query(const query_id_t queryId, DirectQueryCb cb)
  {
    DirectQuery& directQuery = m_RegisteredDirectQueries[queryId];
    for (archetype_id_t archetypeId: directQuery.desiredArchetypes)
      queryArchetype(archetypeId, cb);
  }

  void Registry::tick()
  {
    if (m_Templates.hasPendingRegistration())
    {
      registerCppQueries();
      m_Templates.markRegistered();
    }

    processEvents();

    for(const RegisteredQueryInfo& query: m_RegisteredQueues)
      for(const archetype_id_t archetypeId: query.archetypes)
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
          for(const archetype_id_t archetypeId: query.archetypes)
            queryArchetypeByEvent(event, archetypeId, query.eventCb);
        }
        else
          processEventWithoutArchetypes(event, query.eventCb);
      }

      event->_destr(event);
    }
  }

  void Registry::queryArchetype(const archetype_id_t arch_id, QueryCb cb)
  {
    m_Archetypes.forEachEntityInArchetype(arch_id,
      [cb](ComponentsAccessor& comps)
      {
        cb(comps);
      });
  }

  void Registry::queryArchetype(const archetype_id_t arch_id, const DirectQueryCb& cb)
  {
    m_Archetypes.forEachEntityInArchetype(arch_id,[&cb](ComponentsAccessor& comps)
    {
      cb(comps);
    });
  }

  void Registry::queryArchetypeByEvent(Event* event, const archetype_id_t arch_id, EventQueryCb cb)
  {
    m_Archetypes.forEachEntityInArchetype(arch_id,
      [cb, event](ComponentsAccessor& comps)
      {
        cb(event, comps);
      });
  }

  void Registry::processEventWithoutArchetypes(Event* event, EventQueryCb cb)
  {
     ComponentsAccessor emptyCompAccessor(nullptr, nullptr, {});
     cb(event, emptyCompAccessor);
  }
}