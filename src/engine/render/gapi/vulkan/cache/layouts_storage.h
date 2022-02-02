#pragma once

#include <vulkan/vulkan.hpp>

namespace gapi
{
  struct GraphicsPipelineLayoutDescription;
}

namespace gapi::vulkan
{
  class Device;

  class PipelineLayoutsStorage
  {
    public:
      void Init(Device* device);

      vk::PipelineLayout GetPipelineLayout(const GraphicsPipelineLayoutDescription& desc);

    private:
      Device* m_Device;
      vk::UniquePipelineLayout m_EmptyLayout;
  };
}