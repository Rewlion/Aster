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
    string entry;
    vk::ShaderStageFlagBits stage;
  };

  class ShadersStorage
  {
    public:
      const ShadersStorage& operator=(const ShadersStorage& rvl) = delete;

      void init(Device* device);

      ShaderModuleHandler addModule(const ShadersSystem::ShaderBlob& blob);
      PipelineLayoutHandler addPipelineLayout(const eastl::vector<spirv::v2::DescriptorSet>& dsets);
      const PipelineLayout& getPipelineLayout(const PipelineLayoutHandler h);
      eastl::vector<vk::PipelineShaderStageCreateInfo> getShaderStagesCreateInfos(const eastl::vector<ShaderModuleHandler>& modules) const;

    private:
      const ShaderModule* getModule(const ShaderModuleHandler h) const;

    private:
      Device* m_Device;

      eastl::hash_map<ShaderModuleHandler, ShaderModule> m_ShaderModules;
      eastl::vector<PipelineLayout> m_PipelineLayouts;
  };

};