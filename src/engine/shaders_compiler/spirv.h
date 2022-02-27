#pragma once

#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>

namespace spirv
{
  constexpr uint64_t SHADERS_STAGE_NAME_LEN = 128;

  struct ParsedSpirv
  {
      ParsedSpirv() = default;
      ParsedSpirv(const eastl::vector<char>& binary);

      string m_EntryPoint;
      vk::ShaderStageFlagBits m_Stage;

      uint32_t m_VertexStride = 0;
      Utils::FixedStack<vk::VertexInputAttributeDescription,32> m_VertexAttributeDescriptions;

      bool m_HasInput = false;

      uint32_t m_PushConstantsSize = 0;
  };

  struct InputAssembly
  {
    uint32_t stride;
    Utils::FixedStack<vk::VertexInputAttributeDescription,32> attributes;
  };

  struct Reflection
  {
    char                    entryName[SHADERS_STAGE_NAME_LEN];
    vk::ShaderStageFlagBits stage;
    InputAssembly           inputAssembly;
    uint32_t                pushConstantsSize = 0;
  };

  Reflection Reflect(const eastl::vector<char>& spirv);
}
