#pragma once

#include <stdint.h>

struct ClassField
{
  enum class Type : uint8_t
  {
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    String
  };

  size_t offset;
  const char* name;
  Type type;
};
