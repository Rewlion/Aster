#pragma once

#include <engine/types.h>

#include <stdint.h>
#include <string_view>

constexpr size_t const_strlen(const char* str)
{
  size_t len = 0;
  while(*(str++) != '\0')
    ++len;

  return len;
}

constexpr uint64_t fnv1a(const char* str)
{
  const uint64_t prime = 1099511628211u;
  const uint64_t basis = 14695981039346656037u;


  size_t length = const_strlen(str);

  uint64_t hash = basis;
  for (size_t i = 0; i < length; ++i)
  {
      hash ^= str[i];
      hash *= prime;
  }
  return hash;
}
