#pragma once

#include "constants.h"
#include "ast_types.h"

#include <engine/gapi/resources.h>
#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>
#include <array>

namespace ShadersSystem
{
  struct InputDescription;
}

namespace spirv
{
  struct InputAssembly
  {
    uint32_t stride;
    Utils::FixedStack<vk::VertexInputAttributeDescription,32> attributes;
  };

  enum class BindingType: uint8_t
  {
    None                 = 0,
    Texture2D            = 1,
    Sampler              = 2,
    UniformBufferDynamic = 3
  };

  namespace v2
  {
    struct Binding
    {
      BindingType          type = BindingType::None;
      vk::ShaderStageFlags stages;
      string               name;

      bool operator==(const Binding& rvl) const
      {
        return std::tie(type, stages, name) ==
               std::tie(rvl.type, rvl.stages, rvl.name);
      }
    };

    typedef eastl::vector<vk::DescriptorSetLayoutBinding> DescriptorSet;
    struct Reflection
    {
      eastl::vector<DescriptorSet> descriptorSets;

      bool operator==(const Reflection& rvl) const
      {
        return descriptorSets == rvl.descriptorSets;
      }
    };

    struct InputAssembly
    {
      eastl::vector<vk::VertexInputBindingDescription> buffers;
      eastl::vector<vk::VertexInputAttributeDescription> attributes;

      bool operator==(const InputAssembly& rvl) const
      {
        return std::tie(buffers, attributes) == std::tie(rvl.buffers, rvl.attributes);
      }
    };

    InputAssembly shader_input_to_spirv_ia(const gapi::VertexInputDescription& input);
    Reflection reflect(const eastl::vector<char>& spirv, const gapi::ShaderStage stage);
  }
}
