#pragma once

#include <engine/gapi/vulkan/vulkan.h>

#include <engine/assert.h>
#include <engine/gapi/resources.h>
#include <engine/gapi/vulkan/pipeline_layout.h>
#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>
#include <shaders_compiler/spirv.h>
#include <shaders_compiler/ast_processing_types.h>

#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  struct ShaderModule
  {
    vk::UniqueShaderModule module;
    eastl::vector<spirv::v2::DescriptorSet> descriptorSets;
    string entry;
    vk::ShaderStageFlagBits stage;
  };

  class ShadersStorage
  {
    public:
      void init(Device* device);

      ShaderModuleHandler addModule(const ShadersSystem::ShaderBlob& blob, const spirv::v2::Reflection& reflection);
      const PipelineLayout* getPipelineLayout(const eastl::vector<ShaderModuleHandler>& modules);

    private:
      const ShaderModule* getModule(const ShaderModuleHandler h) const;
      eastl::vector<spirv::v2::DescriptorSet> getModulesDescriptorSets(const eastl::vector<ShaderModuleHandler>& modules) const;

    private:
      Device* m_Device;

      eastl::hash_map<ShaderModuleHandler, ShaderModule> m_ShaderModules;
      eastl::hash_map<string_hash, PipelineLayout> m_PipelineLayouts;
  };

};