#pragma once

#include "hash.h"

template<typename T>
concept ArrayWithHashableStrings = requires(T a)
{
  { str_hash(a[0].c_str()) };
};
