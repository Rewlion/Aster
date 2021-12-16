#pragma once

#include "types.h"

#include <engine/algorithm/hash.h>

constexpr string_hash str_hash(const char* str)
{
  return fnv1a(str);
}
