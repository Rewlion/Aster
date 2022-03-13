#pragma once

#include "shaders_storage.h"

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
      void Init(Device* device);
      vk::Pipeline GetPipeline(const GraphicsPipelineDescription& description, const vk::RenderPass rp, const size_t subpass);
      bool GetPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout);

    private:

    private:
      Device* m_Device = nullptr;
      ShadersStorage m_ShadersStorage;

      eastl::hash_map<string_hash, vk::UniquePipeline> m_Pipelines;
  };
}
