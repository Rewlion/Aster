#pragma once

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>

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

      vk::PipelineLayout GetPipelineLayout(const vk::PipelineLayoutCreateInfo& layout);

    private:
      Device* m_Device;
      vk::UniquePipelineLayout m_EmptyLayout;
      eastl::vector<vk::UniquePipelineLayout> m_Layouts;
  };
}