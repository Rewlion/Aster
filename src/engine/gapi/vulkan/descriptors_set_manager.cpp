#include "descriptors_set_manager.h"
#include "pipeline_layout.h"
#include "result.h"

#include <engine/log.h>
#include <engine/gapi/vulkan/device.h>

namespace gapi::vulkan
{
  DescriptorsSetManager::DescriptorsSetManager(Device& device)
    : m_Device(device)
  {
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

    auto dsPoolUnique = m_Device.getDevice().createDescriptorPoolUnique(ci);
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

  bool DescriptorsSetManager::validateBinding(const size_t set, const size_t binding,
                                              const vk::DescriptorType type) const
  {
    if (set < m_PipelineLayout->dsets.size() &&
        m_PipelineLayout->dsets[set].size() < binding)
    {
      return m_PipelineLayout->dsets[set][binding].descriptorType == type;
    }

    return false;
  }

  vk::DescriptorSet DescriptorsSetManager::acquireSet(const size_t set)
  {
    vk::DescriptorSetAllocateInfo dsAi;
    dsAi.descriptorPool = acquirePool();
    dsAi.descriptorSetCount = 1;
    dsAi.pSetLayouts = &m_PipelineLayout->descriptorSetLayouts[set].get();

    auto res = m_Device.getDevice().allocateDescriptorSets(dsAi);
    if (res.result != vk::Result::eSuccess)
    {
      addPool();
      dsAi.descriptorPool = acquirePool();
      res = m_Device.getDevice().allocateDescriptorSets(dsAi);
      VK_CHECK_RES(res);

      return res.value[0];
    }

    return res.value[0];
  }

  void DescriptorsSetManager::insureSetExistance(const size_t set)
  {
    if (m_BindedDsets[set] == vk::DescriptorSet{})
      m_BindedDsets[set] = acquireSet(set);
  }

  void DescriptorsSetManager::setImage(const vk::ImageView view, const size_t set, const size_t binding)
  {
    m_WriteInfos.push_back(ImageWriteInfo{
      .view = view,
      .set = set,
      .binding = binding
    });
  }

  void DescriptorsSetManager::setSampler(const vk::Sampler sampler, const size_t set, const size_t binding)
  {
    m_WriteInfos.push_back(SamplerWriteInfo{
      .sampler = sampler,
      .set = set,
      .binding = binding
    });
  }

  void DescriptorsSetManager::setUniformBuffer(const vk::Buffer buffer, const size_t set, const size_t binding, const size_t constOffset)
  {
    m_WriteInfos.push_back(UniformBufferWriteInfo{
      .buffer = buffer,
      .constOffset = constOffset,
      .set = set,
      .binding = binding
    });
  }

  namespace
  {
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
  }

  DescriptorsSetManager::WritesDescription DescriptorsSetManager::acquireWrites()
  {
    WritesDescription writes;

    const auto addWrites = [&](size_t set, size_t binding, vk::DescriptorType dsetType, auto setupInfoPtr) {
      if (!validateBinding(set, binding, dsetType))
        return;

      insureSetExistance(set);
      writes.updateSets.insert(set);

      vk::WriteDescriptorSet write;
      write.dstSet = m_BindedDsets[set];
      write.dstBinding = binding;
      write.dstArrayElement = 0;
      write.descriptorCount = 1;
      write.descriptorType = dsetType;
      setupInfoPtr(write);

      writes.writes.push_back(write);
    };

    const auto setupSamplerInfo = [&](vk::Sampler sampler) {
      return [&](vk::WriteDescriptorSet& write) {
        vk::DescriptorImageInfo imgInfo;
        imgInfo.sampler = sampler;
        write.pImageInfo = &writes.imgInfos.back();
      };
    };

    const auto setupImageInfo = [&](vk::ImageView view) {
      return [&](vk::WriteDescriptorSet& write) {
        vk::DescriptorImageInfo imgInfo;
        imgInfo.imageView = view;
        imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        writes.imgInfos.push_back(imgInfo);
        write.pImageInfo = &writes.imgInfos.back();
      };
    };

    const auto setupBufferInfo = [&](vk::Buffer buffer, const size_t constOffset) {
      return [&](vk::WriteDescriptorSet& write) {
        vk::DescriptorBufferInfo bufInfo;
        bufInfo.buffer = buffer;
        bufInfo.offset = constOffset;
        bufInfo.range = VK_WHOLE_SIZE;
        writes.bufInfos.push_back(bufInfo);
        write.pBufferInfo = &writes.bufInfos.back();
      };
    };

    for (const auto& wrInfo: m_WriteInfos)
    {
      std::visit(overload{
        [&](const SamplerWriteInfo& i) {
          addWrites(i.set, i.binding, vk::DescriptorType::eSampler, setupSamplerInfo(i.sampler));
        },
        [&](const ImageWriteInfo& i) {
          addWrites(i.set, i.binding, vk::DescriptorType::eSampledImage, setupImageInfo(i.view));
        },
        [&](const UniformBufferWriteInfo& i) {
          addWrites(i.set, i.binding, vk::DescriptorType::eUniformBuffer, setupBufferInfo(i.buffer, i.constOffset));
        }
      }, wrInfo);
    }

    m_WriteInfos.clear();
    return writes;
  }

  void DescriptorsSetManager::setPipelineLayout(const PipelineLayout* layout)
  {
    if (m_BindedDsets.size() < layout->dsets.size())
      m_BindedDsets.resize(layout->dsets.size());

    if (m_PipelineLayout != nullptr)
    {
      for (size_t i = 0; i < m_PipelineLayout->dsets.size(); ++i)
      {
        if (layout->dsets[i].size() > m_PipelineLayout->dsets[i].size())
        {
          m_BindedDsets[i] = vk::DescriptorSet{};
          continue;
        }

        const bool hasDifferentBindings = std::memcmp(layout->dsets[i].data(),
                                                      m_PipelineLayout->dsets[i].data(),
                                                      layout->dsets[i].size() * sizeof(layout->dsets[i]));
        if (hasDifferentBindings)
          m_BindedDsets[i] = vk::DescriptorSet{};
      }
    }

    m_PipelineLayout = layout;
  }

  void DescriptorsSetManager::updateDescriptorSets(vk::CommandBuffer& cmdBuf)
  {
    if (!m_PipelineLayout)
      return;

    WritesDescription writes = acquireWrites();
    if (!writes.writes.empty())
      m_Device.getDevice().updateDescriptorSets(writes.writes.size(), writes.writes.data(), 0, nullptr);

    for (size_t i = 0; i < m_BindedDsets.size(); ++i)
    {
      const auto& set = m_BindedDsets[i];
      if (set == vk::DescriptorSet{})
      {
        m_BindedDsets[i] = acquireSet(i);
        writes.updateSets.insert(i);
      }
    }

    for (size_t set: writes.updateSets)
    {
      cmdBuf.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                m_PipelineLayout->pipelineLayout.get(),
                                set, 1, &m_BindedDsets[set], 0, nullptr);
    }
  }
}
