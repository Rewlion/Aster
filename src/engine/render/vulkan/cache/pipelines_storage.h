#pragma once

#include <engine/types.h>
#include <engine/render/vulkan/spirv/parser.h>
#include <engine/render/vulkan/traits.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  class PipelineStorage
  {
    struct PipelineDescription
    {
      vk::PipelineShaderStageCreateInfo           stagesInfo[traits::LAST_SHADER];
      vk::PipelineVertexInputStateCreateInfo      pVertexInputState;
      vk::PipelineInputAssemblyStateCreateInfo    pInputAssemblyState;
      vk::PipelineViewportStateCreateInfo         pViewportState;
      vk::PipelineRasterizationStateCreateInfo    pRasterizationState;
      vk::PipelineDepthStencilStateCreateInfo     pDepthStencilState;
      vk::PipelineColorBlendStateCreateInfo       pColorBlendState;
    };

    public:
      void init(Device* device);

      vk::Pipeline getPipeline(const string_hash pipelineName, const vk::RenderPass rp);

    private:
      void createShaderModules();
      void collectPipelineDescriptions();
      void collectPipelineDescription(const DataBlock& ppBlk);

    private:
      Device* m_Device;

      vk::PipelineDepthStencilStateCreateInfo m_DepthStencilStates[traits::MAX_DEPTH_STENCIL_STATES];
      size_t m_ActiveDepthStencilStates = 0;

      struct ShaderModule
      {
        spirv::ParsedSpirv metadata;
        vk::UniqueShaderModule module;
      };
      eastl::hash_map<string_hash, ShaderModule> m_ShaderModules;

      using PipelinesMap = eastl::hash_map<string_hash, vk::UniquePipeline>;
      eastl::hash_map<string_hash, PipelineDescription> m_PipelineDescriptions;
      eastl::hash_map<vk::RenderPass, PipelinesMap> m_Pipelines;
  };

};