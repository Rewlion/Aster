#include "buffer.h"

namespace Vulkan
{
  Buffer::Buffer(vk::Device logicalDevice, vk::UniqueBuffer&& buffer, vk::UniqueDeviceMemory&& memory, vk::DeviceSize memorySize)
    : logicalDevice(logicalDevice)
    , buffer(std::move(buffer))
    , memory(std::move(memory))
    , memorySize(memorySize)
  {
    fullBufferUpdateInfo = vk::DescriptorBufferInfo()
      .setBuffer(this->buffer.get())
      .setOffset(0)
      .setRange(VK_WHOLE_SIZE);
  }

  HostBuffer::HostBuffer(vk::Device logicalDevice, vk::UniqueBuffer&& buffer, vk::UniqueDeviceMemory&& memory, vk::DeviceSize memorySize)
    : Buffer(logicalDevice, std::move(buffer), std::move(memory), memorySize)
    , mappedMemory(nullptr)
  {
  }

  void HostBuffer::UploadMemory(const void* src, vk::DeviceSize size, vk::DeviceSize offset)
  {
    mappedMemory = logicalDevice.mapMemory(memory.get(), offset, size);

    std::memcpy(mappedMemory, src, size);

    logicalDevice.unmapMemory(memory.get());
    mappedMemory = nullptr;
  }
}
