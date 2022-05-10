#pragma once

#include <engine/assert.h>
#include <engine/gapi/resources.h>
#include <engine/gapi/vulkan/pipeline_layout.h>
#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>
#include <shaders_compiler/spirv.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  struct ShaderModule
  {
    spirv::Reflection      metadata;
    vk::UniqueShaderModule module;
  };

  struct ShaderProgramInfo
  {
    Utils::FixedStack<vk::PipelineShaderStageCreateInfo,2>
                                           stages;
    vk::VertexInputBindingDescription      inputBinding;
    vk::PipelineVertexInputStateCreateInfo vertexInput;
    vk::PipelineLayout                     layout;
  };

  class ShadersStorage
  {

    public:
      void init(Device* device);

      const ShaderModule& getShaderModule(const string_hash name);

      const PipelineLayout& getPipelineLayout(const ShaderStagesNames& stages);

      void getShaderProgramInfo(const ShaderStagesNames& stages, ShaderProgramInfo& programInfo);

    private:
      void createShaderModules();

    private:
      Device* m_Device;

      eastl::hash_map<string_hash, ShaderModule> m_ShaderModules;
      eastl::hash_map<string_hash, PipelineLayout> m_PipelineLayouts;
  };

};