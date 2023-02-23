#pragma once

#include <engine/assert.h>

#include "core_types.h"

#include <EASTL/vector_map.h>

namespace ecs
{
  class ComponentsAccessor
  {
    public:
      ComponentsAccessor(std::byte* entity_data,
                         const component_offset_t* offsets,
                         const eastl::vector_map<name_hash_t, component_id_t>& name_to_comp_map);

      template<class T>
      auto get(const name_hash_t name) -> T&;

      template<class T>
      auto get(const name_hash_t name, T&& defValue) -> T&;

    private:
      std::byte* m_EntityData;
      const component_offset_t* m_Offsets;
      const eastl::vector_map<name_hash_t, component_id_t>& m_NameToCompMap;
  };
}

#include "components_accessor.inc.hpp"