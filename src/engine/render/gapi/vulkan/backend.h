#pragma once

#include "indices.h"
#include "device.h"

#include <vulkan/vulkan.hpp>

using UniqueDynamicDbgUtilsMessenger = vk::UniqueHandle<vk::DebugUtilsMessengerEXT,vk::DispatchLoaderDynamic>;

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
      UniqueDynamicDbgUtilsMessenger createDebugMessenger();
      vk::PhysicalDevice getSuitablePhysicalDevice() const;
      QueueIndices getQueueIndices();
      MemoryIndices getMemoryIndices();

    private:
      vk::UniqueInstance m_Instance;
      vk::DispatchLoaderDynamic m_Loader;
      UniqueDynamicDbgUtilsMessenger m_DebugMessenger;
      vk::PhysicalDevice m_PhysicalDevice;
      vk::UniqueSurfaceKHR m_Surface;
      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;
  };
}