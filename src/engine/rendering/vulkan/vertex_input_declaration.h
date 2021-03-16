#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <tuple>

namespace Vulkan 
{
  class VertexInputDeclaration
  {
  public:
    void AddBindingDescription(const uint32_t binding, const uint32_t stride);

    void AddAttributeDescription(const vk::Format format, const uint32_t binding, const uint32_t location, const uint32_t offset);

    const std::vector<vk::VertexInputBindingDescription>& GetBindingDescriptions() const;
    const std::vector<vk::VertexInputAttributeDescription>& GetAttributeDescriptions() const;

    bool operator<(const VertexInputDeclaration& r) const;

  private:
    std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
  };
}