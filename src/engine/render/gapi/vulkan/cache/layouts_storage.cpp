#include "layouts_storage.h"

#include <engine/render/gapi/vulkan/device.h>

#include <boost/functional/hash.hpp>

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

  vk::PipelineLayout PipelineLayoutsStorage::GetPipelineLayout(const vk::PipelineLayoutCreateInfo& ci)
  {
    if (ci.pushConstantRangeCount == 0 && ci.setLayoutCount == 0)
      return m_EmptyLayout.get();

    vk::UniquePipelineLayout layout = m_Device->m_Device->createPipelineLayoutUnique(ci);
    vk::PipelineLayout r = layout.get();
    m_Layouts.push_back(std::move(layout));

    return r;
  }
}