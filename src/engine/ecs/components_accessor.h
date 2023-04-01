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

  class ComponentsAccessorById
  {
    public:
      ComponentsAccessorById() = default;
      ComponentsAccessorById(std::byte* entity_data,
                            const component_offset_t*,
                            const component_type_id_t*,
                            const components_count_t);

      auto operator[](const int i) -> std::byte*;

      auto operator=(const ComponentsAccessorById& r) -> const ComponentsAccessorById&;

      class Iterator
      {
        public:
          Iterator(ComponentsAccessorById& accessor, size_t pos);

          struct Access
          {
            std::byte* compData;
            component_type_id_t typeId;
          };

          auto operator*() const -> Access;
          auto operator++() -> Iterator;
          auto operator==(const Iterator& it) const -> bool;
          auto operator!=(const Iterator& it) const -> bool;

        private:
          ComponentsAccessorById& m_Accessor;
          size_t m_Pos;
      };

      auto begin() -> Iterator;
      auto end() -> Iterator;

    private:
      std::byte* m_EntityData = nullptr;
      const component_offset_t* m_Offsets = nullptr;
      const component_type_id_t* m_Types = nullptr;
      const components_count_t m_CompsCount{0};
  };
}

#include "components_accessor.inc.hpp"