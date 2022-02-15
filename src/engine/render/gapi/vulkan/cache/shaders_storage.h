#pragma once

#include <engine/assert.h>
#include <engine/render/gapi/resources.h>
#include <engine/render/gapi/vulkan/spirv/parser.h>
#include <engine/types.h>
#include <engine/utils/fixed_stack.hpp>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  struct ShaderModule
  {
    spirv::ParsedSpirv     metadata;
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
      void Init(Device* device);

      const ShaderModule& GetShaderModule(const string_hash name);

      vk::PipelineLayout GetPipelineLayout(const ShaderStagesNames& stages);

      void GetShaderProgramInfo(const ShaderStagesNames& stages, ShaderProgramInfo& programInfo);

    private:
      void CreateEmptyPipelineLayout();
      void CreateShaderModules();

    private:
      Device* m_Device;

      eastl::hash_map<string_hash, ShaderModule> m_ShaderModules;
      eastl::hash_map<string_hash, vk::UniquePipelineLayout> m_PipelineLayouts;
  };

};