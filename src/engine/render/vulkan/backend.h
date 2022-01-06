#pragma once

#include "indices.h"
#include "device.h"

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  class Backend
  {
    public:
      void init();
      Device createDevice();

    private:
      eastl::vector<const char*> getValidationLayers();
      vk::UniqueInstance createInstance();
      vk::PhysicalDevice getSuitablePhysicalDevice() const;
      QueueIndices getQueueIndices();
      MemoryIndices getMemoryIndices();

    private:
      vk::UniqueInstance m_Instance;
      vk::PhysicalDevice m_PhysicalDevice;
      vk::UniqueSurfaceKHR m_Surface;
      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;
  };
}