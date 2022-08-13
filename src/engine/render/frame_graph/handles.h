#pragma once

#include <stdint.h>

namespace fg
{
  enum class VirtualResourceHandle: uint32_t { Invalid = (uint32_t)(~0) };
  enum class ResourceHandle: uint32_t { Invalid = (uint32_t)(~0) };
  enum class NodeHandle: uint32_t {Invalid = (uint32_t)(~0) };
}
