#pragma once

#include "constants.h"

#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>
#include <array>

namespace spirv
{
  struct InputAssembly
  {
    uint32_t stride;
    Utils::FixedStack<vk::VertexInputAttributeDescription,32> attributes;
  };

  enum class BindingType: uint8_t
  {
    None      = 0,
    Texture2D = 1,
    Sampler   = 2
  };

  struct Binding
  {
    BindingType          type = BindingType::None;
    vk::ShaderStageFlags stages;
    char                 name[BINDING_NAME_LEN];
  };

  struct ShaderArgument
  {
    Binding bindings[MAX_BINDING_COUNT];

    inline size_t getBindingsCount() const
    {
      size_t size = 0;
      for (size_t i = 0; i < std::size(bindings); ++i)
        if (bindings[i].type != BindingType::None)
          size = i + 1;
      return size;
    }
  };

  struct Reflection
  {
    char                    entryName[SHADERS_STAGE_NAME_LEN];
    vk::ShaderStageFlagBits stage;
    InputAssembly           inputAssembly;
    uint32_t                pushConstantsSize = 0;
    ShaderArgument          shaderArguments[MAX_SETS_COUNT];
  };

  Reflection reflect(const eastl::vector<char>& spirv);
}
