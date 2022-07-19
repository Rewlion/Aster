#pragma once

#include "ast.h"

#include <stdint.h>

namespace ShadersSystem
{
  struct InputDescription
  {
    struct Attribute
    {
      uint32_t offset = 0;
      uint8_t location = 0;
      uint8_t binding = 0;
      AttributeType type;

      bool operator==(const Attribute& rvl) const
      {
        return std::tie(offset, location, binding, type) ==
               std::tie(rvl.offset, rvl.location, rvl.binding, rvl.type);
      }
    };

    struct Buffer
    {
      uint32_t stride = 0;
      uint8_t binding = 0;

      bool operator==(const Buffer& rvl) const
      {
        return std::tie(stride, binding) == std::tie(rvl.stride, rvl.binding);
      }
    };

    bool operator==(const InputDescription& ia) const
    {
      return attributes == ia.attributes &&
        buffers == ia.buffers;
    }

    eastl::vector<Attribute> attributes;
    eastl::vector<Buffer> buffers;
  };
}
