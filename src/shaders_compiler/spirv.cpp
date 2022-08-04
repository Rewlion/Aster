#include "spirv.h"
#include "ast_processing_types.h"

#include <engine/assert.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

#include <vulkan/vulkan.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>

#include <EASTL/functional.h>

namespace spirv
{
  namespace v2
  {
    static vk::DescriptorType get_descriptor_type(const ShadersSystem::ResourceType type)
    {
      switch (type)
      {
        case ShadersSystem::ResourceType::Cbuffer: return vk::DescriptorType::eUniformBuffer;
        case ShadersSystem::ResourceType::Sampler: return vk::DescriptorType::eSampler;
        case ShadersSystem::ResourceType::Texture2D: return vk::DescriptorType::eSampledImage;
        default:
        {
          ASSERT(!"unsupported");
          return {};
        }
      }
    }

    eastl::vector<DescriptorSet> reflect(const eastl::vector<const ShadersSystem::ScopeDeclaration*> scopes, const gapi::ShaderStage stages)
    {
      eastl::vector<DescriptorSet> dsets;
      const auto accessDsetBinding = [&](const size_t set, const size_t binding) -> vk::DescriptorSetLayoutBinding& {
        if (dsets.size() <= set)
          dsets.resize(set+1);

        if (dsets[set].size() <= binding)
          dsets[set].resize(binding+1);

        return dsets[set][binding];
      };

      for (const auto& scope: scopes)
      {
        for (const auto& [_, decl]: scope->declaredResources)
        {
          vk::DescriptorSetLayoutBinding& binding = accessDsetBinding(decl.dset, decl.binding);
          binding.binding = decl.binding;
          binding.descriptorCount = 1;
          binding.descriptorType = get_descriptor_type(decl.type);
          binding.stageFlags = gapi::vulkan::get_shader_stage(stages);
        }
      }

      return dsets;
    }
  }
}
