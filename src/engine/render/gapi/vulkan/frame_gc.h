#pragma once

#include <EASTL/vector.h>

#include "vulkan/vulkan.hpp"

namespace gapi::vulkan
{
  class FrameGarbageCollector
  {
    public:

      inline void addBuffer(vk::UniqueBuffer&& buffer)
      {
        frameResources[frameId].buffers.push_back(std::move(buffer));
      }

      inline void addMemory(vk::UniqueDeviceMemory&& memory)
      {
        frameResources[frameId].memories.push_back(std::move(memory));
      }

      inline void nextFrame()
      {
        frameId += 1;
        frameResources[frameId].buffers.clear();
        frameResources[frameId].memories.clear();
      }

    private:
      struct Resources
      {
        eastl::vector<vk::UniqueBuffer> buffers;
        eastl::vector<vk::UniqueDeviceMemory> memories;
      };

      size_t frameId = 0;
      Resources frameResources[SWAPCHAIN_IMAGES_COUNT];
  };
}
