#pragma once

#include "hash.h"

namespace ecs
{
  struct hashed_name
  {
    hashed_name() = default;
    hashed_name(const string& s)
      : str(s)
      , hash(ecs_name_hash(s.c_str(), s.length()))
    {
    }
    hashed_name(const string_view& s)
      : str(string{s})
      , hash(ecs_name_hash(s.data(), s.length()))
    {
    }
    string str;
    name_hash_t hash;
  };
}
