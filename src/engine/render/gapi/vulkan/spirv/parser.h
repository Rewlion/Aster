#pragma once

#include <engine/types.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>

namespace spirv
{
  struct ParsedSpirv
  {
      ParsedSpirv() = default;
      ParsedSpirv(const eastl::vector<char>& binary);

      string m_EntryPoint;
      vk::ShaderStageFlagBits m_Stage;
      size_t m_VertexAttributesCount = 0;

      uint32_t m_VertexStride = 0;
      vk::VertexInputAttributeDescription m_VertexAttributeDescriptions[32];

      bool m_HasInput = false;

      uint32_t m_PushConstantsSize = 0;
  };
}
