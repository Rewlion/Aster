#pragma once

#include "indices.h"
#include "device.h"

#include <engine/utils/bit_capacity.hpp>

using UniqueDynamicDbgUtilsMessenger = vk::UniqueHandle<vk::DebugUtilsMessengerEXT,vk::DispatchLoaderDynamic>;

namespace gapi::vulkan
{
  class FrameGarbageCollector;

  class Backend
  {
    public:
      void init();
      Device* createDevice(FrameGarbageCollector* frameGc);

    private:
      eastl::vector<const char*> getValidationLayers();
      vk::UniqueInstance createInstance();
      UniqueDynamicDbgUtilsMessenger createDebugMessenger();
      vk::PhysicalDevice getSuitablePhysicalDevice();
      QueueIndices getQueueIndices();
      MemoryIndices getMemoryIndices();

    private:
      enum class InstanceExtensionsBits
      {
        DebugUtils = 0,
        Count
      };
      Utils::BitCapacity<(int)InstanceExtensionsBits::Count> m_SupportedInstanceExts;

      vk::DynamicLoader m_Loader;
      vk::UniqueInstance m_Instance;
      UniqueDynamicDbgUtilsMessenger m_DebugMessenger;
      vk::PhysicalDevice m_PhysicalDevice;
      vk::UniqueSurfaceKHR m_Surface;
      QueueIndices m_QueueIndices;
      MemoryIndices m_MemoryIndices;
  };
}