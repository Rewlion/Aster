#include "template.h"

namespace ecs
{
  TemplateDescription::TemplateDescription(const string& name,
                                           TemplateParentNames&& parents,
                                           TemplateComponentsMap&& comps_map)
    : tmplId(INVALID_TEMPLATE_ID)
    , name(name)
    , parents(std::move(parents))
    , compsMap(comps_map)
  {
  }

  auto Templates::getDescription(const name_hash_t name) -> Templates::DescAccessResult
  {
    const auto it = m_DescMap.find(name);
    const bool valid = it != m_DescMap.end();
    return {it->second, valid};
  }

  auto Templates::addTemplateDesc(TemplateDescription&& desc,
                                  RegisteredComponents& registered_comps) -> bool
  {
    if (registerNewComponents(registered_comps, desc.compsMap)) [[likely]]
    {
      const name_hash_t id = ecs_name_hash(desc.name);
      m_DescMap.insert(eastl::make_pair(id, std::move(desc)));
      return true;
    }

    return false;
  }

  auto Templates::hasTemplateDescription(string_view name) const -> bool
  {
    const name_hash_t id = ecs_name_hash(name);
    return m_DescMap.find(id) != m_DescMap.end();
  }

  auto Templates::getTemplate(const template_id_t id) -> const Template&
  {
    return m_Templates[id];
  }

  auto Templates::requestTemplate(const string_view name,
                                  Archetypes& archetypes,
                                  const RegisteredComponents& registered_comps) -> template_id_t
  {
    const name_hash_t id = ecs_name_hash(name);
    auto [templateDesc, valid] = getDescription(id);
    if (!valid) [[unlikely]]
    {
      logerror("ecs: unknown template {}", name);
      return INVALID_TEMPLATE_ID;
    }

    if (templateDesc.tmplId == INVALID_TEMPLATE_ID) [[unlikely]]
    {
      templateDesc.tmplId = createTemplate(templateDesc, archetypes, registered_comps);
      if (templateDesc.tmplId == INVALID_TEMPLATE_ID) [[unlikely]]
      {
        logerror("ecs: failed to create template `{}`", name);
        return INVALID_TEMPLATE_ID;
      }
    }

    return templateDesc.tmplId;
  }

  auto Templates::hasPendingRegistration() const -> bool
  {
    return m_HasPendingRegistration;
  }

  void Templates::markRegistered()
  {
    m_HasPendingRegistration = false;
  }

  void Templates::iterateOverTemplateDescHierarchy(const TemplateDescription& desc,
                                                   eastl::function<void(const TemplateDescription&)> cb)
  {
    for (const auto& parent: desc.parents)
    {
      const name_hash_t parentNameHash = ecs_name_hash(parent.c_str());
      const auto& parentDesc = m_DescMap.find(parentNameHash)->second;
      iterateOverTemplateDescHierarchy(parentDesc, cb);
      cb(parentDesc);
    }
    cb(desc);
  }

  auto Templates::createTemplate(const TemplateDescription& tmpl_desc,
                                 Archetypes& archetypes,
                                 const RegisteredComponents& registered_comps) -> template_id_t
  {
    eastl::vector_map<registered_component_id_t, const TemplateComponent*> regCompToTmplComp;
    iterateOverTemplateDescHierarchy(tmpl_desc,
      [&registered_comps, &regCompToTmplComp](const TemplateDescription& desc){
      for (const auto& [nameHash, tmplComp]: desc.compsMap.getMap())
      {
        const registered_component_id_t regCompId = registered_comps.getComponentId(nameHash);
        regCompToTmplComp.insert({regCompId, &tmplComp});
      }
    });

    eastl::vector<registered_component_id_t> regCompIds;
    eastl::vector_map<name_hash_t, component_id_t> nameHashToCompId;
    eastl::vector<component_offset_t> compOffsets;
    eastl::vector<component_size_t> compSizes;
    eastl::vector<component_type_id_t> compTypes;

    regCompIds.reserve(regCompToTmplComp.size());
    nameHashToCompId.reserve(regCompToTmplComp.size());
    compOffsets.reserve(regCompToTmplComp.size());
    compSizes.reserve(regCompToTmplComp.size());
    compTypes.reserve(regCompToTmplComp.size());
    
    size_t entityEnd = 0;

    for (const auto [regCompId, pTmplComp]: regCompToTmplComp)
    {
      regCompIds.push_back(regCompId);

      component_id_t compId = (component_id_t)compTypes.size();
      nameHashToCompId.insert({pTmplComp->getNameHash(), compId});

      const component_size_t compSize = (component_size_t)pTmplComp->getMeta()->size;
      compOffsets.push_back((component_offset_t)entityEnd);
      compSizes.push_back(compSize);
      compTypes.push_back(pTmplComp->getMeta()->typeId);
      entityEnd += compSize;
      Sys::align(entityEnd, 16);
    }

    const entity_size_t entitySize = (entity_size_t)entityEnd;

    const archetype_id_t archId = archetypes.addArchetype(
      entitySize,
      std::move(nameHashToCompId),
      std::move(regCompIds),
      std::move(compOffsets),
      std::move(compSizes),
      std::move(compTypes));

    auto initBuffer = eastl::unique_ptr<std::byte>(new std::byte[entitySize]);

    template_id_t tmplId = (template_id_t)m_Templates.size();
    m_Templates.push_back();
    m_Templates.back() = Template{
      std::move(initBuffer),
      archId,
      std::move(regCompToTmplComp)};

    m_HasPendingRegistration = true;

    return tmplId;
  }

  auto Templates::registerNewComponents(RegisteredComponents& registered_comps,
                                        const TemplateComponentsMap& comps_map) -> bool
  {
    bool valid = true;
    for (const auto& [nameHash, tmplComp]: comps_map.getMap())
    {
      const RegisteredComponent* regComp = registered_comps.getComponent(nameHash);
      if (regComp)
      {
        if (regComp->typeMeta.typeId != tmplComp.getMeta()->typeId) [[unlikely]]
        {
          logerror("ecs: invalid component `{}`, it has component: {} but registered type is: {}",
            comps_map.getComponentName(nameHash),
            tmplComp.getMeta()->typeName,
            regComp->typeMeta.typeName);
          valid = false;
        }
      }
      else
        registered_comps.addComponent(tmplComp.getMeta()->typeId, nameHash, string{comps_map.getComponentName(nameHash)});
    }
    return valid;
  }
}
