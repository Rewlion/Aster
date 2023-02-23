#pragma once

#include <engine/types.h>
#include <engine/utils/strong_typedef.h>

namespace ecs
{
  STRONG_SCALAR_TYPEDEF(uint16_t, archetype_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, chunk_eid_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, chunk_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, component_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, component_offset_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, component_size_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, component_type_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, components_count_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, entity_size_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, query_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, registered_component_id_t);
  STRONG_SCALAR_TYPEDEF(uint16_t, template_id_t);
  STRONG_SCALAR_TYPEDEF(uint32_t, components_set_hash_t);
  STRONG_SCALAR_TYPEDEF(uint32_t, name_hash_t);

  constexpr query_id_t INVALID_QUERY_ID = query_id_t{-1};
  constexpr registered_component_id_t INVALID_REG_COMPONENT_ID = registered_component_id_t{-1};
  constexpr component_type_id_t INVALID_COMPONENT_TYPE_ID = component_type_id_t{-1};
  constexpr template_id_t INVALID_TEMPLATE_ID = template_id_t{-1};
}
