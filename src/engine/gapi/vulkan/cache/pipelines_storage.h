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
      void init(Device* device);
      vk::Pipeline getPipeline(const GraphicsPipelineDescription& description, const vk::RenderPass rp, const size_t subpass);
      const PipelineLayout* getPipelineLayout(const eastl::vector<ShaderModuleHandler>& modules);
      ShaderModuleHandler addModule(const ShadersSystem::ShaderBlob& blob, const spirv::v2::Reflection& reflection);

    private:
      Device* m_Device = nullptr;
      ShadersStorage m_ShadersStorage;

      eastl::hash_map<string_hash, vk::UniquePipeline> m_Pipelines;
  };
}
