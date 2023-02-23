#pragma once

#include "entity_storage.h"
#include "components_accessor.h"

#include <EASTL/bonus/tuple_vector.h>
#include <EASTL/vector_multimap.h>
#include <EASTL/functional.h>
#include <EASTL/span.h>
#define XXH_INLINE_ALL
#include <xxhash/xxhash.h>

namespace ecs
{
  class Archetypes
  {
    public:
      auto addArchetype(entity_size_t entity_size,
                        eastl::vector_map<name_hash_t, component_id_t>&& name_to_comp_map,
                        eastl::vector<registered_component_id_t>&& comp_ids,                  
                        eastl::vector<component_offset_t>&& offsets,
                        eastl::vector<component_size_t>&& sizes,
                        eastl::vector<component_type_id_t>&& types) -> archetype_id_t;

    #define ARCH_TUPLE_R_ACCESS(type, methodName) \
      const type& get ## methodName (const archetype_id_t id) const\
      {\
        return m_Archetypes.get<type>()[id];\
      }
    #define ARCH_TUPLE_RW_ACCESS(type, methodName) \
      type& get ## methodName (const archetype_id_t id)\
      {\
        return m_Archetypes.get<type>()[id];\
      }
    #define _MAP(a,b) eastl::vector_map<a,b>
    #define _VEC(a) eastl::vector<a>

      ARCH_TUPLE_R_ACCESS (entity_size_t,                     EntitySize)
      ARCH_TUPLE_R_ACCESS (_MAP(name_hash_t, component_id_t), NameToComponentsMap)
      ARCH_TUPLE_RW_ACCESS(EntityStorage,                     EntityStorage)
      ARCH_TUPLE_R_ACCESS (components_count_t,                ComponentsCount)
      ARCH_TUPLE_R_ACCESS (components_set_hash_t,             ComponentsSetHash)
      ARCH_TUPLE_R_ACCESS (_VEC(registered_component_id_t),   ComponentIds)
      ARCH_TUPLE_R_ACCESS (_VEC(component_offset_t),          ComponentOffsets)
      ARCH_TUPLE_R_ACCESS (_VEC(component_size_t),            ComponentSizes)
      ARCH_TUPLE_R_ACCESS (_VEC(component_type_id_t),         ComponentTypes)
    
    #undef _VEC
    #undef _MAP
    #undef ARCH_TUPLE_R_ACCESS_ALL
    #undef ARCH_TUPLE_R_ACCESS
    #undef ARCH_TUPLE_RW_ACCESS

      auto getArchetypesCount() const -> archetype_id_t;

      void forEachEntityInArchetype(const archetype_id_t arch_id,
                                    const eastl::function<void(ComponentsAccessor&)>& cb);

      auto findArchetypesWithComponents(const eastl::vector<registered_component_id_t>& comps)
        -> eastl::vector<archetype_id_t>;

    private:
      auto hashCompIds(const eastl::vector<registered_component_id_t>& ids) const
        -> components_set_hash_t;

    private:
      eastl::tuple_vector<
        entity_size_t,
        eastl::vector_map<name_hash_t, component_id_t>,
        EntityStorage,
        components_count_t,
        components_set_hash_t,
        eastl::vector<registered_component_id_t>,
        eastl::vector<component_offset_t>,
        eastl::vector<component_size_t>,
        eastl::vector<component_type_id_t>
      > m_Archetypes;

      eastl::vector_multimap<components_set_hash_t, archetype_id_t> m_CompsHashToArchetypes;
  };
}
