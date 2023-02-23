#include "components_accessor.h"

namespace ecs
{
  ComponentsAccessor::ComponentsAccessor(std::byte* entity_data,
                                         const component_offset_t* offsets,
                                         const eastl::vector_map<name_hash_t, component_id_t>& name_to_comp_map)
    : m_EntityData(entity_data)
    , m_Offsets(offsets)
    , m_NameToCompMap(name_to_comp_map)
  {
  }
}
