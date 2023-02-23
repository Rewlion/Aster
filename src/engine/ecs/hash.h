#pragma once

#include <engine/algorithm/hash.h>

#include "core_types.h"

consteval
auto compile_ecs_name_hash(const char* str) -> ecs::name_hash_t
{
  return ecs::name_hash_t{fnv1a<uint32_t>(str, const_strlen(str))};
}

consteval
auto compile_ecs_name_hash(string_view str) -> ecs::name_hash_t
{
  return ecs::name_hash_t{fnv1a<uint32_t>(str.data(), str.size())};
}

constexpr
auto ecs_name_hash(const char* str) -> ecs::name_hash_t
{
  return ecs::name_hash_t{fnv1a<uint32_t>(str, const_strlen(str))};
}

constexpr
auto ecs_name_hash(const char* str, const size_t len) -> ecs::name_hash_t
{
  return ecs::name_hash_t{fnv1a<uint32_t>(str, len)};
}

constexpr
auto ecs_name_hash(string_view str) -> ecs::name_hash_t
{
  return ecs::name_hash_t{fnv1a<uint32_t>(str.data(), str.size())};
}
