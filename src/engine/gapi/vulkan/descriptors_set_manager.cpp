#include "descriptors_set_manager.h"
#include "pipeline_layout.h"
#include "result.h"

#include <engine/log.h>
#include <engine/gapi/vulkan/device.h>

namespace gapi::vulkan
{
  void DescriptorsSetManager::init(Device* device)
  {
    m_Device = device;
  }

  void DescriptorsSetManager::addPool()
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

  vk::DescriptorPool& DescriptorsSetManager::acquirePool()
  {
    if (m_Pools.size() == 0)
      addPool();

    return m_Pools[m_PoolId].get();
  }

  void DescriptorsSetManager::setImage(const vk::ImageView imgView, const size_t set, const size_t binding)
  {
    m_Sets[set].bindings[binding].imgView = imgView;
    m_Sets[set].bindings[binding].type = vk::DescriptorType::eSampledImage;
    vk::DescriptorImageInfo imgInfo;
    imgInfo.imageView = imgView;
    imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    m_Sets[set].bindings[binding].imgInfo = imgInfo;

    m_DirtySets.set(set);
  }

  void DescriptorsSetManager::setSampler(const vk::Sampler sampler, const size_t set, const size_t binding)
  {
    m_Sets[set].bindings[binding].sampler = sampler;
    m_Sets[set].bindings[binding].type = vk::DescriptorType::eSampler;
    vk::DescriptorImageInfo imgInfo;
    imgInfo.sampler = sampler;
    m_Sets[set].bindings[binding].imgInfo = imgInfo;

    m_DirtySets.set(set);
  }

  void DescriptorsSetManager::setUniformBuffer(const vk::Buffer buffer, const size_t set, const size_t binding, const size_t constOffset, const size_t dynamicOffset)
  {
    m_Sets[set].bindings[binding].buffer = buffer;
    m_Sets[set].bindings[binding].type = vk::DescriptorType::eUniformBufferDynamic;
    m_Sets[set].bindings[binding].dynamicOffset = dynamicOffset;
    vk::DescriptorBufferInfo bufInfo;
    bufInfo.buffer = buffer;
    bufInfo.offset = constOffset;
    bufInfo.range = VK_WHOLE_SIZE;
    m_Sets[set].bindings[binding].bufInfo = bufInfo;

    m_DirtySets.set(set);
  }

  void DescriptorsSetManager::setPipelineLayout(const PipelineLayout* layout)
  {
    if (m_PipelineLayout != nullptr)
    {
      for (size_t set = 0; set < std::size(m_PipelineLayout->sets); ++set)
      {
        const size_t bindingsCount = std::max(layout->sets[set].getBindingsCount(),
                                              m_PipelineLayout->sets[set].getBindingsCount());
        const bool hasDifferentBindings = std::memcmp(&layout->sets[set],
                                                      &m_PipelineLayout->sets[set],
                                                      bindingsCount * sizeof(layout->sets[set].bindings[0]));
        if (hasDifferentBindings)
          m_DirtySets.set(set);
      }
    }
    m_PipelineLayout = layout;
  }

  bool DescriptorsSetManager::validateBinding(const size_t set, const size_t binding)
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

      case spirv::BindingType::UniformBufferDynamic:
      {
        if (currentBindingType == vk::DescriptorType::eUniformBufferDynamic)
          return true;
        break;
      }
    }

    //logerror("vulkan: can't set [set:{}, binding:{}], pipeline hasn't declared such binding", set, binding);
    return false;
  }

  vk::DescriptorSet DescriptorsSetManager::acquireSet(const size_t set)
  {
    vk::DescriptorSetAllocateInfo dsAi;
    dsAi.descriptorPool = acquirePool();
    dsAi.descriptorSetCount = 1;
    dsAi.pSetLayouts = &m_PipelineLayout->descriptorSetLayouts[set].get();

    auto res = m_Device->m_Device->allocateDescriptorSets(dsAi);
    if (res.result != vk::Result::eSuccess)
    {
      addPool();
      dsAi.descriptorPool = acquirePool();
      res =  m_Device->m_Device->allocateDescriptorSets(dsAi);
      VK_CHECK_RES(res);

      return res.value[0];
    }

    m_ActiveSets.set(set);
    return res.value[0];
  }

  vk::WriteDescriptorSet DescriptorsSetManager::acquireWriteDescriptorSet(const size_t nSet, const size_t nBinding)
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

      case vk::DescriptorType::eUniformBufferDynamic:
      {
        write.pBufferInfo = &binding.bufInfo;
        break;
      }

      default: ASSERT(!"unsupported descriptor type");
    }

    return write;
  }

  void DescriptorsSetManager::updateDescriptorSets(vk::CommandBuffer& cmdBuf)
  {
    eastl::vector<vk::WriteDescriptorSet> writes;
    writes.reserve(spirv::MAX_SETS_COUNT * spirv::MAX_BINDING_COUNT);

    for (size_t set = 0; set < spirv::MAX_SETS_COUNT; ++set)
    {
      if (m_DirtySets.isSet(set))
      {
        m_ActiveDescriptorSets[set] = acquireSet(set);
        for (size_t binding = 0; binding < m_Sets[set].getBindingsCount(); ++binding)
        {
          if (!m_Sets[set].bindings[binding].type.has_value())
            continue;

          if (!validateBinding(set, binding))
          {
            m_Sets[set].bindings[binding].type = std::nullopt;
            continue;
          }

          const vk::WriteDescriptorSet write = acquireWriteDescriptorSet(set, binding);
          writes.push_back(write);
        }
      }
      else
        if (!m_ActiveSets.isSet(set))
        {
          m_ActiveDescriptorSets[set] = acquireSet(set);
          m_DirtySets.set(set);
        }
    }

    if (writes.size() > 0)
    {
      m_Device->m_Device->updateDescriptorSets(writes.size(), writes.data(), 0, nullptr);

      for (size_t set = 0; set < spirv::MAX_SETS_COUNT; ++set)
        if (m_DirtySets.isSet(set))
        {
          const auto dynamicOffsets = acquireDynamicOffsets(set);
          cmdBuf.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_PipelineLayout->pipelineLayout.get(),
                                    set, 1, &m_ActiveDescriptorSets[set], dynamicOffsets.getSize(), dynamicOffsets.getData());
        }
    }

    m_DirtySets.resetAll();
  }

  Utils::FixedStack<uint32_t, spirv::MAX_BINDING_COUNT> DescriptorsSetManager::acquireDynamicOffsets(const size_t nSet)
  {
    Utils::FixedStack<uint32_t, spirv::MAX_BINDING_COUNT> dynamicOffsets;

    const auto setDescription = m_Sets[nSet];
    for (size_t i = 0; i < std::size(setDescription.bindings); ++i)
    {
      const auto& bindingDescription = setDescription.bindings[i];
      if (bindingDescription.type.has_value() &&
        (bindingDescription.type.value() == vk::DescriptorType::eUniformBufferDynamic))
      {
        dynamicOffsets.push(bindingDescription.dynamicOffset);
      }
    }

    return dynamicOffsets;
  }

  void DescriptorsSetManager::reset()
  {
    m_Pools.clear();
    m_DirtySets.resetAll();
  }
}
