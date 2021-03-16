#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <memory>

namespace Vulkan
{
  class Buffer
  {
  public:
    Buffer() = default;

    Buffer(vk::Device logicalDevice, vk::UniqueBuffer&& buffer, vk::UniqueDeviceMemory&& memory, vk::DeviceSize memorySize);

    inline vk::Buffer GetBuffer() const
    {
      return buffer.get();
    }

    inline const vk::DescriptorBufferInfo& GetFullBufferUpdateInfo() const
    {
      return fullBufferUpdateInfo;
    }

  protected:
    vk::Device logicalDevice;
    vk::UniqueBuffer buffer;
    vk::UniqueDeviceMemory memory;
    vk::DeviceSize memorySize;

    vk::DescriptorBufferInfo fullBufferUpdateInfo;
  };

  class HostBuffer : public Buffer
  {
  public:
    HostBuffer() = default;

    HostBuffer(vk::Device logicalDevice, vk::UniqueBuffer&& buffer, vk::UniqueDeviceMemory&& memory, vk::DeviceSize memorySize);

    inline void* Map()
    {
      mappedMemory = logicalDevice.mapMemory(memory.get(), 0, memorySize);
      return mappedMemory;
    }

    inline void Unmap()
    {
      logicalDevice.unmapMemory(memory.get());
      mappedMemory = nullptr;
    }

    void UploadMemory(const void* src, vk::DeviceSize size, vk::DeviceSize offset);

    inline void* GetMappedMemory() const
    {
      return mappedMemory;
    }

  private:
    void* mappedMemory;
  };
}
