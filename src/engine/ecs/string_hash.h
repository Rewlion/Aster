#pragma once

#include <engine/algorithm/hash.h>

constexpr uint64_t str_hash(const char* str)
{
  return fnv1a(str);
}
