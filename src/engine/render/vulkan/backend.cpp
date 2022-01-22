#include "backend.h"
#include "surface.h"

#include <engine/assert.h>
#include <engine/settings.h>
#include <engine/log.h>
#include <engine/window.h>

#include <EASTL/vector.h>
#include <EASTL/vector_set.h>

namespace gapi::vulkan
{
  eastl::vector<const char*> Backend::getValidationLayers()
  {
    eastl::vector<const char*> validationLayers;
    if (Engine::GetAppSettings()->GetChildBlock("vulkan")->GetBool("validation", false))
      validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

    return validationLayers;
  }

  vk::UniqueInstance Backend::createInstance()
  {
    eastl::vector<const char*> validationLayers = getValidationLayers();
    const auto appName = Engine::GetAppSettings()->GetText("app_name");

    const auto appInfo = vk::ApplicationInfo()
      .setPApplicationName(appName.c_str())
      .setApplicationVersion(VK_MAKE_VERSION(-1, 0, 0))
      .setPEngineName(appName.c_str())
      .setEngineVersion(VK_MAKE_VERSION(-1, 0, 0))
      .setApiVersion(VK_API_VERSION_1_1);

    const char* instanceExtensions[] = {
      "VK_KHR_surface",
      "VK_KHR_win32_surface"
    };

    const auto instanceCreateInfo = vk::InstanceCreateInfo()
      .setPApplicationInfo(&appInfo)
      .setEnabledExtensionCount(std::size(instanceExtensions))
      .setPpEnabledExtensionNames(instanceExtensions)
      .setEnabledLayerCount((uint32_t)validationLayers.size())
      .setPpEnabledLayerNames(validationLayers.data());

    return vk::createInstanceUnique(instanceCreateInfo);
  }

  void Backend::init()
  {
    m_Instance = createInstance();
    m_PhysicalDevice = getSuitablePhysicalDevice();
    m_Surface = createPlatformSurface(*m_Instance);
    m_QueueIndices = getQueueIndices();
    m_MemoryIndices = getMemoryIndices();
  }

  vk::PhysicalDevice Backend::getSuitablePhysicalDevice() const
  {
    vk::PhysicalDevice physicalDevice = nullptr;
    for (const vk::PhysicalDevice& device : m_Instance->enumeratePhysicalDevices())
    {
      vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

      if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
      {
        physicalDevice = device;
        break;
      }
    }

    ASSERT(physicalDevice != vk::PhysicalDevice{});

    return physicalDevice;
  }

  QueueIndices Backend::getQueueIndices()
  {
    std::vector<vk::QueueFamilyProperties> queueFamilies = m_PhysicalDevice.getQueueFamilyProperties();
    QueueIndices indices;

    for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++)
    {
      vk::QueueFamilyProperties properties = queueFamilies[familyIndex];
      const auto validQueue = [&properties](const size_t& index, const vk::QueueFlagBits& desiredQueueType)
                              {
                                return index == size_t(-1) && properties.queueCount > 0 && properties.queueFlags & desiredQueueType;
                              };

      if (validQueue(indices.graphics, vk::QueueFlagBits::eGraphics))
        indices.graphics = familyIndex;

      if (validQueue(indices.transfer, vk::QueueFlagBits::eTransfer))
        indices.transfer = familyIndex;

      if (indices.present == size_t(-1) && properties.queueCount > 0 && m_PhysicalDevice.getSurfaceSupportKHR(familyIndex, *m_Surface))
        indices.present = familyIndex;
    }

    ASSERT(indices.graphics != size_t(-1) && indices.present != size_t(-1));

    if (indices.transfer == size_t(-1))
      indices.transfer = indices.graphics;

    return indices;
  }

  MemoryIndices Backend::getMemoryIndices()
  {
    vk::PhysicalDeviceMemoryProperties memoryProperties = m_PhysicalDevice.getMemoryProperties();
    MemoryIndices memoryIndices;

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
      const vk::MemoryType& memType = memoryProperties.memoryTypes[i];
      const auto setMemoryIndex = [&memType, &i](size_t& index, const vk::Flags<vk::MemoryPropertyFlagBits>& flags)
                                  {
                                    if (index == size_t(-1) && memType.propertyFlags & flags)
                                      index = i;
                                  };
      setMemoryIndex(memoryIndices.deviceLocalMemory,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

      setMemoryIndex(memoryIndices.stagingMemory,
        vk::MemoryPropertyFlagBits::eDeviceLocal);
    }

    ASSERT(memoryIndices.stagingMemory != size_t(-1) && memoryIndices.deviceLocalMemory != size_t(-1));
    return memoryIndices;
  }

  Device Backend::createDevice()
  {
    eastl::vector_set<size_t> uniqueQueueFamilyIndices = { m_QueueIndices.graphics, m_QueueIndices.present, m_QueueIndices.transfer };

    eastl::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilyIndices)
    {
      auto queueCreateInfo = vk::DeviceQueueCreateInfo()
        .setQueueFamilyIndex(queueFamily)
        .setQueueCount(1)
        .setPQueuePriorities(&queuePriority);

      queueCreateInfos.push_back(queueCreateInfo);
    }

    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    const auto deviceCreateInfo = vk::DeviceCreateInfo()
        .setQueueCreateInfoCount(queueCreateInfos.size())
        .setPQueueCreateInfos(queueCreateInfos.data())
        .setPpEnabledExtensionNames(deviceExtensions)
        .setEnabledExtensionCount(std::size(deviceExtensions));

    vk::UniqueDevice device =  m_PhysicalDevice.createDeviceUnique(deviceCreateInfo);

    const int2 wndSize = Engine::Window::GetWindowSize();

    Device::CreateInfo deviceCi{
      .instance = *m_Instance,
      .surface = *m_Surface,
      .device = eastl::move(device),
      .queueIndices = m_QueueIndices,
      .memoryIndices = m_MemoryIndices,

      .surfaceCapabilities = m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface),
      .surfaceFormats = m_PhysicalDevice.getSurfaceFormatsKHR(*m_Surface),
      .surfacePresentModes = m_PhysicalDevice.getSurfacePresentModesKHR(*m_Surface),

      .swapchainImageExtent = vk::Extent2D{(uint32_t)wndSize.x, (uint32_t)wndSize.y}
    };
    return Device(std::move(deviceCi));
  }
}