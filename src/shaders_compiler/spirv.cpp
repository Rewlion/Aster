#include "spirv.h"
#include "ast_processing_types.h"

#include <engine/assert.h>
#include <engine/gapi/vulkan/gapi_to_vk.h>
#include <engine/log.h>

#include <vulkan/vulkan.hpp>
#include <SPIRV-Cross/spirv_glsl.hpp>

#include <EASTL/functional.h>
#include <EASTL/sort.h>

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
        case ShadersSystem::ResourceType::Buffer: return vk::DescriptorType::eUniformTexelBuffer;
        case ShadersSystem::ResourceType::Texture2D: return vk::DescriptorType::eSampledImage;
        case ShadersSystem::ResourceType::Texture3D: return vk::DescriptorType::eSampledImage;
        case ShadersSystem::ResourceType::TextureCube: return vk::DescriptorType::eSampledImage;
        case ShadersSystem::ResourceType::RWStructuredBuffer: return vk::DescriptorType::eStorageBuffer;
        case ShadersSystem::ResourceType::RWBuffer: return vk::DescriptorType::eStorageTexelBuffer;
        case ShadersSystem::ResourceType::RWTexture3D: return vk::DescriptorType::eStorageImage;
        default:
        {
          ASSERT(!"unsupported");
          return {};
        }
      }
    }

    eastl::vector<DescriptorSet> reflect(const eastl::vector<const ShadersSystem::ScopeDeclaration*> scopes, const gapi::ShaderStage stages)
    {
      struct BindingInfo
      {
        int remapId;
        string_view firstScopeDelcarer;
        string_view firstResourceDeclarer;
      };

      using binding_id_t = int;
      using DsetBindingsRemap = eastl::vector_map<binding_id_t, BindingInfo>;

      eastl::vector<DescriptorSet> dsets;
      eastl::vector<DsetBindingsRemap> remaps;

      const auto dumpRes = [] (const string_view res_name, const string_view scope_name)
      {
        return fmt::format("[resource:{} scope:{}]", res_name, scope_name);
      };

      const auto accessDsetBinding = [&](const size_t set, const size_t binding, const string_view res_name, const string_view scope_name)
        -> DescriptorSetBinding&
      {
        if (dsets.size() <= set)
        {
          dsets.resize(set+1);
          remaps.resize(set+1);
        }

        auto& remap = remaps[set];
        auto bindingInfoIt = remap.find(binding);
        if (bindingInfoIt != remap.end())
        {
          logerror("\n>>> overlapping resources in set:{} binding:{}:\n>>> {}\n>>> and\n>>> {}",
            set, binding,
            dumpRes(res_name, scope_name),
            dumpRes(bindingInfoIt->second.firstResourceDeclarer, bindingInfoIt->second.firstScopeDelcarer));

          return dsets[set][bindingInfoIt->second.remapId];
        }
        else
        {
          const int bindingId = dsets[set].size();
          remap.insert({
            binding,
            {
              .remapId = bindingId,
              .firstScopeDelcarer = scope_name,
              .firstResourceDeclarer = res_name
            }
          });
          return dsets[set].push_back();
        }
      };

      for (const auto& scope: scopes)
      {
        for (const auto& [_, decl]: scope->declaredResources)
        {
          DescriptorSetBinding& binding = accessDsetBinding(decl.dset, decl.binding, decl.name, scope->name);
          binding.vk.binding = decl.binding;
          binding.vk.descriptorCount = 1;
          binding.vk.descriptorType = get_descriptor_type(decl.type);
          binding.vk.stageFlags = gapi::vulkan::get_shader_stage(stages);
          binding.resourceType = decl.type;
        }
      }

      for (auto& bindings: dsets)
      {
        eastl::sort(bindings.begin(), bindings.end(), [](auto& l, auto& r) {
          return l.vk.binding < r.vk.binding;
        });
      }

      return dsets;
    }
  }
}
