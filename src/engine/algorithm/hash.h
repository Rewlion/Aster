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

template<class T>
constexpr T fnv1a(const char* str, size_t length)
{
  const auto calc = [str, length](auto basis, auto prime)
  {
    auto hash = basis;
    for (size_t i = 0; i < length; ++i)
    {
        hash ^= str[i];
        hash *= prime;
    }
    return hash;
  };

  static_assert(std::is_same_v<uint64_t, T> || std::is_same_v<uint32_t, T>);

  if constexpr (std::is_same_v<uint64_t, T>)
  {
    const uint64_t basis = 14695981039346656037u;
    const uint64_t prime = 1099511628211u;

    return calc(basis, prime);
  }
  else if constexpr (std::is_same_v<uint32_t, T>)
  {
    const uint32_t basis = 0x01000193;
    const uint32_t prime = 0x811c9dc5;

    return calc(basis, prime);
  }
}

constexpr string_hash str_hash(const char* str)
{
  return fnv1a<uint64_t>(str, const_strlen(str));
}

constexpr string_hash str_hash(const std::string_view str)
{
  return fnv1a<uint64_t>(str.data(), str.size());
}