#pragma once

#include <engine/utils/strong_typedef.h>
#include <engine/algorithm/hash.h>

namespace console
{
  STRONG_SCALAR_TYPEDEF(uint32_t, cmd_hash_t);

  constexpr
  auto cmd_name_hash(const char* str) -> cmd_hash_t
  {
    return cmd_hash_t{fnv1a<uint32_t>(str, const_strlen(str))};
  }
}
