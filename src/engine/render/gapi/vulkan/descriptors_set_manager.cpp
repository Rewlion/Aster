#include "descriptors_set_manager.h"
#include "pipeline_layout.h"
#include "result.h"

#include <engine/log.h>
#include <engine/render/gapi/vulkan/device.h>

namespace gapi::vulkan
{
  void DescriptorsSetManager::Init(Device* device)
  {
    m_Device = device;
  }

  void DescriptorsSetManager::AddPool()
  {
    vk::DescriptorPoolSize sizes[] = {
      vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 100)
    };

    vk::DescriptorPoolCreateInfo ci{};
    ci.maxSets = 1024;
    ci.pPoolSizes = sizes;
    ci.poolSizeCount = std::size(sizes);

    auto dsPoolUnique = m_Device->m_Device->createDescriptorPoolUnique(ci);
    VK_CHECK_RES(dsPoolUnique);

    m_Pools.push_back(std::move(dsPoolUnique.value));

    if (m_Pools.size() == 1)
      m_PoolId = 0;
    else
      m_PoolId += 1;
  }

  vk::DescriptorPool& DescriptorsSetManager::AcquirePool()
  {
    if (m_Pools.size() == 0)
      AddPool();

    return m_Pools[m_PoolId].get();
  }

  void DescriptorsSetManager::SetImage(const vk::ImageView imgView, const size_t set, const size_t binding)
  {
    m_Sets[set].bindings[binding].imgView = imgView;
    m_Sets[set].bindings[binding].type = vk::DescriptorType::eSampledImage;
    vk::DescriptorImageInfo imgInfo;
    imgInfo.imageView = imgView;
    imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    m_Sets[set].bindings[binding].imgInfo = imgInfo;

    m_DirtySets.Set(set);
  }

  void DescriptorsSetManager::SetSampler(const vk::Sampler sampler, const size_t set, const size_t binding)
  {
    m_Sets[set].bindings[binding].sampler = sampler;
    m_Sets[set].bindings[binding].type = vk::DescriptorType::eSampler;
    vk::DescriptorImageInfo imgInfo;
    imgInfo.sampler = sampler;
    m_Sets[set].bindings[binding].imgInfo = imgInfo;

    m_DirtySets.Set(set);
  }

  void DescriptorsSetManager::SetPipelineLayout(const PipelineLayout* layout)
  {
    m_PipelineLayout = layout;

    //m_DirtySets.ResetAll();
  }

  bool DescriptorsSetManager::ValidateBinding(const size_t set, const size_t binding)
  {
    const spirv::BindingType pipelineBindingType = m_PipelineLayout->sets[set].bindings[binding].type;
    const vk::DescriptorType currentBindingType = m_Sets[set].bindings[binding].type.value();

    switch (pipelineBindingType)
    {
      case spirv::BindingType::Sampler:
      {
        if (currentBindingType == vk::DescriptorType::eSampler)
          return true;
        break;
      }

      case spirv::BindingType::Texture2D:
      {
        if (currentBindingType == vk::DescriptorType::eSampledImage)
          return true;
        break;
      }
    }

    logerror("vulkan: can't set [set:{}, binding:{}], pipeline hasn't declared such binding", set, binding);
    return false;
  }

  vk::DescriptorSet DescriptorsSetManager::AcquireSet(const size_t set)
  {
    vk::DescriptorSetAllocateInfo dsAi;
    dsAi.descriptorPool = AcquirePool();
    dsAi.descriptorSetCount = 1;
    dsAi.pSetLayouts = &m_PipelineLayout->descriptorSetLayouts[set].get();

    auto res = m_Device->m_Device->allocateDescriptorSets(dsAi);
    if (res.result != vk::Result::eSuccess)
    {
      AddPool();
      dsAi.descriptorPool = AcquirePool();
      res =  m_Device->m_Device->allocateDescriptorSets(dsAi);
      VK_CHECK_RES(res);

      return res.value[0];
    }

    return res.value[0];
  }

  vk::WriteDescriptorSet DescriptorsSetManager::AcquireWriteDescriptorSet(const size_t nSet, const size_t nBinding)
  {
    vk::WriteDescriptorSet write;
    Binding& binding = m_Sets[nSet].bindings[nBinding];

    write.dstSet = m_ActiveDescriptorSets[nSet];
    write.dstBinding = nBinding;
    write.dstArrayElement = 0;
    write.descriptorCount = 1;
    write.descriptorType =  binding.type.value();

    switch (binding.type.value())
    {
      case vk::DescriptorType::eSampler:
      case vk::DescriptorType::eSampledImage:
      {
        write.pImageInfo = &binding.imgInfo;
        break;
      }

      default: ASSERT(!"unsupported descriptor type");
    }

    return write;
  }

  void DescriptorsSetManager::UpdateDescriptorSets(vk::CommandBuffer& cmdBuf)
  {
    eastl::vector<vk::WriteDescriptorSet> writes;
    writes.reserve(spirv::MAX_SETS_COUNT * spirv::MAX_BINDING_COUNT);

    for (size_t set = 0; set < spirv::MAX_SETS_COUNT; ++set)
    {
      if (m_DirtySets.IsSet(set))
      {
        m_ActiveDescriptorSets[set] = AcquireSet(set);
        for (size_t binding = 0; binding < m_Sets[set].GetBindingsCount(); ++binding)
        {
          if (!m_Sets[set].bindings[binding].type.has_value())
            continue;

          if (!ValidateBinding(set, binding))
          {
            m_Sets[set].bindings[binding].type = std::nullopt;
            continue;
          }

          const vk::WriteDescriptorSet write = AcquireWriteDescriptorSet(set, binding);
          writes.push_back(write);
        }
      }
      else
        if (!m_ActiveSets.IsSet(set))
        {
          m_ActiveDescriptorSets[set] = AcquireSet(set);
          m_ActiveSets.Set(set);
          m_DirtySets.Set(set);
        }
    }

    if (writes.size() > 0)
    {
      m_Device->m_Device->updateDescriptorSets(writes.size(), writes.data(), 0, nullptr);

      for (size_t set = 0; set < spirv::MAX_SETS_COUNT; ++set)
        if (m_DirtySets.IsSet(set))
          cmdBuf.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_PipelineLayout->pipelineLayout.get(),
                                    set, 1, &m_ActiveDescriptorSets[set], 0, nullptr);
    }

    m_DirtySets.ResetAll();
  }

  void DescriptorsSetManager::Reset()
  {
    m_Pools.clear();
    m_DirtySets.ResetAll();
  }
}
