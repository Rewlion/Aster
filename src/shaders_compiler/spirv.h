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
  struct ScopeDeclaration;
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
    struct DescriptorSetBinding
    {
      vk::DescriptorSetLayoutBinding vk;
      ShadersSystem::ResourceType resourceType;

      bool operator==(const DescriptorSetBinding& rvl) const
      {
        return std::memcmp(this, &rvl, sizeof(*this));
      }
    };

    typedef eastl::vector<DescriptorSetBinding> DescriptorSet;
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
    eastl::vector<DescriptorSet> reflect(const eastl::vector<const ShadersSystem::ScopeDeclaration*> scopes, const gapi::ShaderStage stages);
  }
}
