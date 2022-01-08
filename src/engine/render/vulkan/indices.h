#pragma once

namespace gapi::vulkan
{
  struct QueueIndices
  {
    size_t graphics = -1;
    size_t present = -1;
    size_t transfer = -1;
  };

  struct MemoryIndices
  {
    size_t stagingMemory = -1;
    size_t deviceLocalMemory = -1;
  };
}
