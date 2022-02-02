#include "layouts_storage.h"

#include <engine/render/gapi/vulkan/device.h>

namespace gapi::vulkan
{
  void PipelineLayoutsStorage::Init(Device* device)
  {
    m_Device = device;

    vk::PipelineLayoutCreateInfo ci;
    ci.pSetLayouts = nullptr;
    ci.pPushConstantRanges = nullptr;
    ci.setLayoutCount = 0;
    ci.pushConstantRangeCount = 0;

    m_EmptyLayout = m_Device->m_Device->createPipelineLayoutUnique(ci);
  }

  vk::PipelineLayout PipelineLayoutsStorage::GetPipelineLayout(const GraphicsPipelineLayoutDescription& desc)
  {
    return m_EmptyLayout.get();
  }
}