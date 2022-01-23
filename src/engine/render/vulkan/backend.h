#pragma once

#include "indices.h"
#include "device.h"

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  class Backend
  {
    public:
      void Init();
      Device CreateDevice();

    private:
      eastl::vector<const char*> GetValidationLayers();
      vk::UniqueInstance CreateInstance();
      vk::PhysicalDevice GetSuitablePhysicalDevice() const;
      QueueIndices GetQueueIndices();
      MemoryIndices GetMemoryIndices();

    private:
      vk::UniqueInstance m_Instance;
      vk::PhysicalDevice m_PhysicalDevice;
      vk::UniqueSurfaceKHR m_Surface;
      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;
  };
}