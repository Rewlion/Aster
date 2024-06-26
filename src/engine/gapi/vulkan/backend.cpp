#include "backend.h"
#include "surface.h"
#include "result.h"

#include <engine/assert.h>
#include <engine/settings.h>
#include <engine/log.h>
#include <engine/window.h>

#include <EASTL/vector.h>
#include <EASTL/vector_set.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace gapi::vulkan
{
  static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
  {
    if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT &&
        messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
      Backend* backend = reinterpret_cast<Backend*>(pUserData);

      logerror("vulkan: {}\n{}", pCallbackData->pMessage, backend->getDbgContext());
    }

    return VK_FALSE;
  }

  UniqueDynamicDbgUtilsMessenger Backend::createDebugMessenger()
  {
    vk::DebugUtilsMessengerCreateInfoEXT ci;
    ci.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    ci.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
    ci.pfnUserCallback = VkDebugCallback;
    ci.pUserData = this;

    auto res = m_Instance->createDebugUtilsMessengerEXTUnique(ci);
    VK_CHECK_RES(res);

    return std::move(res.value);
  }

  vk::UniqueInstance Backend::createInstance()
  {
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = m_Loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
    VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

    loginfo("vulkan: instance supported extensions:");
    for (const auto& ext: vk::enumerateInstanceExtensionProperties().value)
    {
      loginfo(">> {} [v:{}]", ext.extensionName.data(), ext.specVersion);
      if (!std::strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ext.extensionName.data()))
        m_SupportedInstanceExts.set((size_t)InstanceExtensionsBits::DebugUtils);
    }

    const string_view appName = Engine::get_app_settings().getVariable<string_view>("app_name");

    const auto appInfo = vk::ApplicationInfo()
      .setPApplicationName(appName.data())
      .setApplicationVersion(VK_MAKE_VERSION(-1, 0, 0))
      .setPEngineName(appName.data())
      .setEngineVersion(VK_MAKE_VERSION(-1, 0, 0))
      .setApiVersion(ENGINE_VK_VERSION);

    eastl::vector<const char*> instanceExtensions = {
      "VK_KHR_surface",
      "VK_KHR_win32_surface",
      "VK_KHR_get_physical_device_properties2"
    };

  #if CFG_DEBUG_UTILS
    if (m_SupportedInstanceExts.isSet((size_t)InstanceExtensionsBits::DebugUtils))
      instanceExtensions.push_back("VK_EXT_debug_utils");
  #endif
    
    loginfo("vulkan: enabled instance extensions:");
    for (const char* ext: instanceExtensions)
      loginfo(">> {}", ext);

    const char* validationLayers[] = {
      "VK_LAYER_KHRONOS_validation"
    };

    const uint32_t validationLayersCount =
    #if CFG_DEBUG_VALIDATION
      Engine::get_app_settings()
        .getScope("vulkan")
        .getVariableOr<bool>("validation", false) ? (sizeof(validationLayers) / sizeof(validationLayers[0])) : 0;
    #else
      0;
    #endif

    const auto instanceCreateInfo = vk::InstanceCreateInfo()
      .setPApplicationInfo(&appInfo)
      .setEnabledExtensionCount(instanceExtensions.size())
      .setPpEnabledExtensionNames(instanceExtensions.data())
      .setEnabledLayerCount(validationLayersCount)
      .setPpEnabledLayerNames(validationLayers);

    auto res = vk::createInstanceUnique(instanceCreateInfo);
    VK_CHECK_RES(res);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(res.value.get());
    return std::move(res.value);
  }
 
  void Backend::init()
  {
    m_Instance = createInstance();

  #if CFG_DEBUG_UTILS
    m_DebugMessenger = createDebugMessenger();
  #endif

    m_PhysicalDevice = getSuitablePhysicalDevice();
    m_Surface = createPlatformSurface(*m_Instance);
    m_QueueIndices = getQueueIndices();
  }

  vk::PhysicalDevice Backend::getSuitablePhysicalDevice()
  {
    vk::PhysicalDevice physicalDevice = nullptr;
    const auto devices = m_Instance->enumeratePhysicalDevices();
    VK_CHECK_RES(devices);
    for (const vk::PhysicalDevice& device : devices.value)
    {
      vk::PhysicalDeviceProperties deviceProperties = device.getProperties();

      if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
      {
        physicalDevice = device;
        break;
      }
    }

    ASSERT(physicalDevice != vk::PhysicalDevice{});

    loginfo("vulkan: device supported extensions:");
    for (const auto& ext: physicalDevice.enumerateDeviceExtensionProperties().value)
    {
      loginfo(">> {} [v:{}]", ext.extensionName.data(), ext.specVersion);
    }

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

      const auto hasSurfaceSupportKHR = m_PhysicalDevice.getSurfaceSupportKHR(familyIndex, *m_Surface);
      VK_CHECK_RES(hasSurfaceSupportKHR);

      if (indices.present == size_t(-1) && properties.queueCount > 0 && hasSurfaceSupportKHR.value)
        indices.present = familyIndex;
    }

    ASSERT(indices.graphics != size_t(-1) && indices.present != size_t(-1));

    if (indices.transfer == size_t(-1))
      indices.transfer = indices.graphics;

    return indices;
  }

  Device* Backend::createDevice(FrameGarbageCollector* frameGc)
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

    std::array<const char*, 4> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      "VK_KHR_timeline_semaphore",
      VK_EXT_CALIBRATED_TIMESTAMPS_EXTENSION_NAME,
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
    };

    loginfo("vulkan: enabled device extensions:");
    for (const char* enabledExt: deviceExtensions)
      loginfo("  {}", enabledExt);

    vk::PhysicalDeviceFeatures features;
    features.geometryShader = true;
    features.tessellationShader = true;
    features.fillModeNonSolid = true;
    features.fragmentStoresAndAtomics = true;
    features.independentBlend = true;

    loginfo("vulkan: supported bindless resources");
    vk::PhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures;
    descriptorIndexingFeatures.runtimeDescriptorArray = true;
    descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = true;
    descriptorIndexingFeatures.descriptorBindingPartiallyBound = true;
    descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount = true;

    const auto deviceCreateInfo = vk::DeviceCreateInfo()
        .setQueueCreateInfoCount(queueCreateInfos.size())
        .setPQueueCreateInfos(queueCreateInfos.data())
        .setPpEnabledExtensionNames(deviceExtensions.data())
        .setEnabledExtensionCount(deviceExtensions.size())
        .setPEnabledFeatures(&features)
        .setPNext(&descriptorIndexingFeatures);

    auto device =  m_PhysicalDevice.createDeviceUnique(deviceCreateInfo);
    VK_CHECK_RES(device);

    VULKAN_HPP_DEFAULT_DISPATCHER.init(device.value.get());

    const int2 wndSize = Engine::Window::get_window_size();

    auto surfaceCaps = m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface);
    VK_CHECK_RES(surfaceCaps);
    auto surfaceFormats = m_PhysicalDevice.getSurfaceFormatsKHR(*m_Surface);
    VK_CHECK_RES(surfaceFormats);
    auto surfacePresentModes = m_PhysicalDevice.getSurfacePresentModesKHR(*m_Surface);
    VK_CHECK_RES(surfacePresentModes);

    Device::Capabilities caps;
  #if CFG_DEBUG_UTILS
    if (m_SupportedInstanceExts.isSet((size_t)InstanceExtensionsBits::DebugUtils) &&
      Engine::get_app_settings().getScope("graphics").getVariableOr<bool>("debug_marks", false))
    {
      caps.set((size_t)Device::CapabilitiesBits::DebugMarks);
    }
  #endif

    Device::CreateInfo deviceCi{
      .physicalDevice = m_PhysicalDevice,
      .instance = *m_Instance,
      .surface = *m_Surface,
      .device = eastl::move(device.value),
      .deviceProperties = m_PhysicalDevice.getProperties(),
      .queueIndices = m_QueueIndices,

      .surfaceCapabilities = std::move(surfaceCaps.value),
      .surfaceFormats = std::move(surfaceFormats.value),
      .surfacePresentModes = std::move(surfacePresentModes.value),

      .swapchainImageExtent = vk::Extent2D{(uint32_t)wndSize.x, (uint32_t)wndSize.y},
      .capabilities = caps
    };
    return new Device(std::move(deviceCi), frameGc);
  }

  void Backend::pushDbgContext(string&& ctx)
  {
  #ifdef DEBUG
    m_DebugContexts.push_back(std::move(ctx));
  #endif
  }

  void Backend::popDbgContext()
  {
  #ifdef DEBUG
    m_DebugContexts.pop_back();
  #endif
  }

  string Backend::getDbgContext() const
  {
  #ifdef DEBUG
    string ctx = "[debug context: ";
    for (auto& c : m_DebugContexts)
      ctx += c + " | ";

    return ctx;
  #else
    return {};
  #endif
  }
}