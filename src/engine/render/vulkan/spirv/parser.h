#pragma once

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>

namespace spirv
{
  struct ParsedSpirv
  {
      ParsedSpirv() = default;
      ParsedSpirv(const eastl::vector<char>& binary);

      vk::PipelineShaderStageCreateInfo m_StageInfo;
      size_t m_VertexAttributesCount = 0;

      uint32_t m_VertexStride = 0;
      vk::VertexInputAttributeDescription m_VertexAttributeDescriptions[32];
  };
}