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

    struct DescriptorSet
    {
      eastl::vector<Binding> bindings;

      bool operator==(const DescriptorSet& rvl) const
      {
        return bindings == rvl.bindings;
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

    struct Reflection
    {
      string                       entry;
      vk::ShaderStageFlagBits      stage;
      eastl::vector<DescriptorSet> dsets;

      bool operator==(const Reflection& rvl) const
      {
        return std::tie(entry, stage, dsets) ==
               std::tie(rvl.entry, rvl.stage, rvl.dsets);
      }
    };

    InputAssembly shader_input_to_spirv_ia(const gapi::VertexInputDescription& input);
    Reflection reflect(const eastl::vector<char>& spirv, const gapi::ShaderStage stage, const string& entry);
  }
}
