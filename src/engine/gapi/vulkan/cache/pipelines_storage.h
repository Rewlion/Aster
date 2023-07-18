#pragma once

#include "shaders_storage.h"

#include <shaders_compiler/ast_processing_types.h>

namespace gapi
{
  struct GraphicsPipelineDescription;
}

namespace gapi::vulkan
{
  class Device;
  class PipelinesStorage
  {
    public:
      const PipelinesStorage& operator=(const PipelinesStorage& rvl) = delete;

      void init(Device* device);
      vk::Pipeline getPipeline(const GraphicsPipelineDescription& description, const vk::RenderPass rp, const size_t subpass);
      PipelineLayoutHandler addPipelineLayout(const eastl::vector<spirv::v2::DescriptorSet>& dsets);
      const PipelineLayout& getPipelineLayout(const PipelineLayoutHandler h);
      ShaderModuleHandler addModule(const ShadersSystem::ShaderBlob& blob);
      void clear();
    private:
      Device* m_Device = nullptr;
      ShadersStorage m_ShadersStorage;

      eastl::hash_map<string_hash, vk::UniquePipeline> m_Pipelines;
  };
}
