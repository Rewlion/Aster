#include "registry.h"
#include "components_accessor.h"
#include "ecs_events.h"

#include <engine/log.h>
#include <engine/utils/string.h>

#include <ranges>

namespace ecs
{
  void Registry::addTemplate(const string& name,
                            TemplateParentNames&& parents,
                            TemplateComponentsMap&& comps_map)
  {
    auto* tDesc = m_Templates.getDescription(name);
    if (tDesc)
    {
      if (tDesc->parents != parents)
      {
        const auto dumpParents = [](const TemplateParentNames& parents){
          string res;
          for (auto p: parents)
            if (res.empty())
              res = p;
            else
              res = fmt::format("{} {}", res, p);
          return res;
        };
        logerror("ecs: failed to register template `{}`: it's already registered with different parents. Registered:[{}] New:[{}]",
          name, dumpParents(tDesc->parents), dumpParents(parents));
      }
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

  auto Registry::getEntityTemplateName(const EntityId eid) const -> string_view
  {
    const auto eInfoIt = m_EntityInfosMap.find(eid.getId());
    if (eInfoIt != m_EntityInfosMap.end() && eInfoIt->second.eid == eid)
      return eInfoIt->second.templateName;
    
    return "";
  }

  auto Registry::createEntity(const string_view tmpl_name, EntityComponents&& inits) -> EntityId
  {
    return recreateEntity({}, tmpl_name, std::move(inits));
  }

  auto Registry::recreateEntity(const EntityId eid, const string_view tmpl_name, EntityComponents&& inits) -> EntityId
  {
    const template_id_t tmplId = m_Templates.requestTemplate(tmpl_name, m_Archetypes, m_RegisteredComponents);
    if (tmplId == INVALID_TEMPLATE_ID) [[unlikely]]
    {
      logerror("ecs: failed to create entity from `{}`", tmpl_name);
      return {};
    }

    const bool isRecreating = eid.isValid();

    const Template& tmpl = m_Templates.getTemplate(tmplId);
    const archetype_id_t archId = tmpl.archetypeId;

    const EntityInfo* srcEinfo = nullptr;
    const eastl::vector<registered_component_id_t>* srcCompIds = nullptr;

    const auto srcEinfoIt = m_EntityInfosMap.find(eid.getId());
    if (srcEinfoIt != m_EntityInfosMap.end())
      srcCompIds = &m_Archetypes.getComponentIds(srcEinfoIt->second.archId);

    const auto& offsets = m_Archetypes.getComponentOffsets(archId);
    const auto& types = m_Archetypes.getComponentTypes(archId);
    const auto& sizes = m_Archetypes.getComponentSizes(archId);
    const auto& compIds = m_Archetypes.getComponentIds(archId);
    const components_count_t compsCount = m_Archetypes.getComponentsCount(archId);

    eastl::vector_map<registered_component_id_t, TemplateComponent*> compIdToInit;
    for (EntityComponentInit& init: inits.getInits())
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

    eastl::vector<int> compToSrcCompRemap;
    if (isRecreating)
    {
      compToSrcCompRemap.reserve(compsCount);
      for (auto compId: compIds)
      {
        auto it = eastl::lower_bound(srcCompIds->begin(), srcCompIds->end(), compId);
        const bool found = it != srcCompIds->end() && compId == *it;
        compToSrcCompRemap.push_back(
          found ? (int)(it - srcCompIds->begin()) : -1
        );
      }
    }

    std::byte* initBuffer = tmpl.initBuffer.get();
    ComponentsAccessorById srcCompsAccessor;
    if (isRecreating)
    {
      srcCompsAccessor = m_Archetypes.accessEntityComponents(
        srcEinfoIt->second.archId, srcEinfoIt->second.chunkId, srcEinfoIt->second.chunkEid);
    }

    for (components_count_t i{0}; i < compsCount; ++i)
    {
      std::byte* compDataForInit = initBuffer + offsets[i];
      const registered_component_id_t compId = compIds[i];

      auto initIt = compIdToInit.find(compId);
      const bool hasInitData = initIt != compIdToInit.end();

      const int srcCompId = isRecreating ? compToSrcCompRemap[i] : -1;
      const bool moveFromSrc = !hasInitData && isRecreating && (srcCompId != -1);

      const auto* typeMeta = get_meta_storage().getMeta(types[i]);
      const auto& typeMgr = typeMeta->manager;
      if (moveFromSrc)
      {
        void* tmplInitData = srcCompsAccessor[srcCompId];
        typeMgr->moveConstructor(compDataForInit, tmplInitData);
      }
      else
      {
        void* initData = hasInitData ? initIt->second->getData() : nullptr;
        const void* tmplInitData = tmpl.regCompToTmplComp.find(compIds[i])->second->getData();

        if (initData || tmplInitData)
        {
          if (typeMeta->isTrivial)
          {
            const void* src = initData ? initData : tmplInitData;
            std::memcpy(compDataForInit, src, typeMeta->size);
          }
          else
          {
            typeMgr->constructor(compDataForInit);
            if (initData)
              typeMgr->moveConstructor(compDataForInit, initData);
            else
              typeMgr->constructor(compDataForInit, tmplInitData);
          }
        }
        else
          typeMgr->constructor(compDataForInit);
      }
    }

    if (isRecreating)
      destroyEntity(eid);

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
        .archId = archId,
        .chunkId = chunkId,
        .chunkEid = chunkEid,
        .templateName = string{tmpl_name}
      }
    ));

    OnEntityCreated evt;
    evt.eid = newEid;
    sendEvent(std::move(evt), newEid);

    return newEid;
  }

  void Registry::destroyEntity(const EntityId eid)
  {
    auto it = m_EntityInfosMap.find(eid.id);
    if (it == m_EntityInfosMap.end())
      return;

    EntityInfo& eInfo = it->second;
    if (eInfo.eid.generation != eid.generation)
      return;

    for(auto[compData, typeId] : 
      m_Archetypes.accessEntityComponents(eInfo.archId, eInfo.chunkId, eInfo.chunkEid))
    {
      const auto& typeMeta = get_meta_storage().getMeta(typeId);
      if (!typeMeta->isTrivial)
        typeMeta->manager->destructor(compData);
    }

    m_Archetypes
      .getEntityStorage(eInfo.archId)
      .removeEntity(eInfo.chunkId, eInfo.chunkEid);
  }

  auto Registry::findDesiredArchetypes(const QueryComponents& queryComponents)
    -> eastl::vector<archetype_id_t>
  {
    eastl::vector<registered_component_id_t> comps;
    comps.reserve(queryComponents.size());
    for (const auto& qc: queryComponents)
    {
      const auto [rc, compId] = m_RegisteredComponents.getComponentAndId(qc.nameHash);
      if (!rc || rc->typeMeta.typeId != qc.typeId)
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
    const bool isEventWithoutArchetype = components.empty();
    const eastl::vector<archetype_id_t> desiredArchetypes = findDesiredArchetypes(components);

    if (desiredArchetypes.empty() && !isEventWithoutArchetype)
      return;

    auto it = m_EventHandleQueries.find(event);
    if (it != m_EventHandleQueries.end())
    {
      it->second.push_back(RegisteredEventQueryInfo{
        .eventCb = cb,
        .archetypes = eastl::move(desiredArchetypes)
      });
    }
  }

  void Registry::query(const query_id_t query_id, DirectQueryCb cb)
  {
    DirectQuery& directQuery = m_RegisteredDirectQueries[query_id];
    for (archetype_id_t archetypeId: directQuery.desiredArchetypes)
      queryArchetype(archetypeId, cb);
  }

  void Registry::query(const EntityId eid, DirectQueryCb cb)
  {
    const auto it = m_EntityInfosMap.find(eid.getId());
    if (it != m_EntityInfosMap.end())
    {
      const auto& eInfo = it->second;
      m_Archetypes.accessEntityInArchetype(eInfo.archId, eInfo.chunkId, eInfo.chunkEid, cb);
    }
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

  void Registry::processDirectEvent(Event* event)
  {
    eastl::vector<RegisteredEventQueryInfo>& queries = m_EventHandleQueries[event->eventNameHash];

    const auto it = m_EntityInfosMap.find(event->receiver.getId());
    if ((it != m_EntityInfosMap.end()) && (it->second.eid == event->receiver))
    {
      const archetype_id_t archetypeId = it->second.archId;
      for (const auto& query: queries)
      {
        const bool hasValidArch = eastl::find(query.archetypes.begin(), query.archetypes.end(), archetypeId)
                                    != query.archetypes.end();
        if (hasValidArch)
        {
          m_Archetypes.accessEntityInArchetype(
            archetypeId,it->second.chunkId, it->second.chunkEid,
            [cb = query.eventCb, event](ComponentsAccessor& comps)
            {
              cb(event, comps);
            });
        }
      }
    }
  }

  void Registry::processBroadcastEvent(Event* event)
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
  }

  void Registry::processEvents()
  {
    auto eventQueue = std::move(m_EventsQueue);
    while (Event* event = eventQueue.popEvent())
    {
      if (event->receiver.isValid())
        processDirectEvent(event);
      else
        processBroadcastEvent(event);

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