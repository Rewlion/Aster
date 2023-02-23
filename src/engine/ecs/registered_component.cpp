#include "registered_component.h"

namespace ecs
{
  auto RegisteredComponents::addComponent(const component_type_id_t type_id,
                                          const name_hash_t name_hash,
                                          const string& name) -> registered_component_id_t
  {
    const TypeMeta* meta = get_meta_storage().getMeta(type_id);
    if (!meta) [[unlikely]]
      return INVALID_REG_COMPONENT_ID;

    const registered_component_id_t compId = (registered_component_id_t)m_Components.size();

    m_Components.push_back({*meta, name_hash});
    m_CompsNames.insert(eastl::make_pair(name_hash, string{name}));
    m_CompNameToId.insert({name_hash, compId});

    return compId;
  }

  auto RegisteredComponents::getComponentAndId(const name_hash_t hash) const
    -> ComponentAccess
  {
    const name_hash_t nameId = (name_hash_t)(hash);
    const auto it = m_CompNameToId.find(nameId);
    if (it != m_CompNameToId.end()) [[likely]]
      return {
        .component = &m_Components[it->second],
        .compId = it->second
      };
    else
      return {
        .component = nullptr,
        .compId = INVALID_REG_COMPONENT_ID
      };
  }

  auto RegisteredComponents::getComponent(const name_hash_t hash) const
    -> const RegisteredComponent*
  {
    const name_hash_t nameId = (name_hash_t)(hash);
    const auto it = m_CompNameToId.find(nameId);
    if (it != m_CompNameToId.end()) [[likely]]
      return &m_Components[it->second];
    else
      return nullptr;
  }

  auto RegisteredComponents::getComponentId(const name_hash_t hash) const
    -> registered_component_id_t
  {
    const name_hash_t nameId = (name_hash_t)(hash);
    const auto it = m_CompNameToId.find(nameId);
    return it != m_CompNameToId.end() ? it->second : INVALID_REG_COMPONENT_ID;
  }

  auto RegisteredComponents::getComponentName(const name_hash_t id)
    -> string_view
  {
    const auto it = m_CompsNames.find(id);
    return it != m_CompsNames.end() ? it->second : string_view{"error:unknown component"};
  }
}
