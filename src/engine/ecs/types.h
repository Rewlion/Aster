#pragma once

#include <EASTL/vector_map.h>

#include <stdint.h>

namespace Engine::ECS
{
  using string_hash = uint64_t;
  using component_name_id = string_hash;
  using chunk_id = uint16_t;
  using block_id = uint16_t;
  using component_type_id = string_hash;
  using archetype_id = uint16_t;
  using type_placement_id = uint16_t;
  using template_name_id = string_hash;
  using query_id = uint16_t;
  using event_hash_name = string_hash;

  constexpr size_t CHUNK_SIZE = 16 * 1024;

  constexpr string_hash INVALID_HASH = (string_hash)-1;
  constexpr chunk_id INVALID_CHUNK_ID = (chunk_id)-1;
  constexpr block_id INVALID_BLOCK_ID = (block_id)-1;
  constexpr component_type_id INVALID_COMPONENT_TYPE_ID = (component_type_id)-1;
  constexpr component_name_id INVALID_COMPONENT_NAME_ID = (component_name_id)-1;
  constexpr archetype_id INVALID_ARCHETYPE_ID = (archetype_id)-1;
  constexpr type_placement_id INVALID_TYPE_PLACEMENT_ID = (type_placement_id)-1;
  constexpr template_name_id INVALID_TEMPLATE_NAME_ID = (template_name_id)-1;
  constexpr query_id INVALID_QUERY_ID = (query_id)-1;
}
