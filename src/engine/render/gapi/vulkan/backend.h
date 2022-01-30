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
      void Init();
      Device CreateDevice();

    private:
      eastl::vector<const char*> GetValidationLayers();
      vk::UniqueInstance CreateInstance();
      UniqueDynamicDbgUtilsMessenger CreateDebugMessenger();
      vk::PhysicalDevice GetSuitablePhysicalDevice() const;
      QueueIndices GetQueueIndices();
      MemoryIndices GetMemoryIndices();

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