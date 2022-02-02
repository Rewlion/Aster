#pragma once

#include "shaders_storage.h"
#include "layouts_storage.h"

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

    private:

    private:
      Device* m_Device = nullptr;
      ShadersStorage m_ShadersStorage;
      PipelineLayoutsStorage m_LayoutsStorage;

      eastl::hash_map<string_hash, vk::UniquePipeline> m_Pipelines;
  };
}
