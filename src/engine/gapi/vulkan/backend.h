#pragma once

#include "indices.h"
#include "device.h"

#include <engine/utils/bit_capacity.hpp>

#include <EASTL/vector.h>

using UniqueDynamicDbgUtilsMessenger = vk::UniqueHandle<vk::DebugUtilsMessengerEXT,vk::DispatchLoaderDynamic>;

namespace gapi::vulkan
{
  class FrameGarbageCollector;

  class Backend
  {
    public:
      void init();
      Device* createDevice(FrameGarbageCollector* frameGc);

      void pushDbgContext(string&&);
      void popDbgContext();
      string getDbgContext() const;

    private:
      vk::UniqueInstance createInstance();
      UniqueDynamicDbgUtilsMessenger createDebugMessenger();
      vk::PhysicalDevice getSuitablePhysicalDevice();
      QueueIndices getQueueIndices();

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

    #ifdef DEBUG
      eastl::vector<string> m_DebugContexts;
    #endif
  };
}