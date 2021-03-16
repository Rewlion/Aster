#include "vertex_input_declaration.h"

namespace vk
{
  bool operator<(const vk::VertexInputBindingDescription& l, const vk::VertexInputBindingDescription& r)
  {
    return std::tie(l.binding, l.inputRate, l.stride) < std::tie(r.binding, r.inputRate, r.stride);
  }

  bool operator<(const vk::VertexInputAttributeDescription& l, const vk::VertexInputAttributeDescription& r)
  {
    return std::tie(l.binding, l.format, l.location, l.offset) < std::tie(r.binding, r.format, r.location, r.offset);
  }
}

namespace Vulkan
{
  void VertexInputDeclaration::AddBindingDescription(const uint32_t binding, const uint32_t stride)
  {
    const auto bindingDsc = vk::VertexInputBindingDescription()
      .setBinding(binding)
      .setStride(stride);

    bindingDescriptions.push_back(bindingDsc);
  }

  void VertexInputDeclaration::AddAttributeDescription(const vk::Format format, const uint32_t binding, const uint32_t location, const uint32_t offset)
  {
    const auto attributeDsc = vk::VertexInputAttributeDescription()
      .setFormat(format)
      .setBinding(binding)
      .setLocation(location)
      .setOffset(offset);

    attributeDescriptions.push_back(attributeDsc);
  }

  const std::vector<vk::VertexInputBindingDescription>& VertexInputDeclaration::GetBindingDescriptions() const
  {
    return bindingDescriptions;
  }

  const std::vector<vk::VertexInputAttributeDescription>& VertexInputDeclaration::GetAttributeDescriptions() const
  {
    return attributeDescriptions;
  }

  bool VertexInputDeclaration::operator<(const VertexInputDeclaration& r) const
  {
    return std::tie(bindingDescriptions, attributeDescriptions) < std::tie(r.bindingDescriptions, r.attributeDescriptions);
  }
}