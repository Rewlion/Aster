#pragma once

#include "core_types.h"
#include "type_meta.h"
#include "hash.h"

#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

namespace ecs
{
  struct RegisteredComponent
  {
    const TypeMeta& typeMeta;
    name_hash_t nameId;
  };

  class RegisteredComponents
  {
    public:
      auto addComponent(const component_type_id_t type_id,
                        const name_hash_t name_hash,
                        const string& name) -> registered_component_id_t;

      struct ComponentAccess
      {
        const RegisteredComponent* component;
        const registered_component_id_t compId;
      };

      auto getComponentAndId(const name_hash_t hash) const -> ComponentAccess;
      auto getComponent(const name_hash_t hash) const -> const RegisteredComponent*;
      auto getComponentId(const name_hash_t hash) const -> registered_component_id_t;
      auto getComponentName(const name_hash_t id) -> string_view;

    private:
      eastl::vector<RegisteredComponent> m_Components;
      eastl::vector_map<name_hash_t, registered_component_id_t> m_CompNameToId;
      eastl::vector_map<name_hash_t, string> m_CompsNames;
  };
}
