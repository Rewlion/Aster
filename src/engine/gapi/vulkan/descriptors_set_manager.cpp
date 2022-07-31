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

  DescriptorsSetManager::DescriptorsSetManager(DescriptorsSetManager&& rvl)
    : m_Device(rvl.m_Device)
    , m_PipelineLayout(rvl.m_PipelineLayout)
    , m_Pools(std::move(rvl.m_Pools))
    , m_PoolId(rvl.m_PoolId)
    , m_BindedDsets(std::move(rvl.m_BindedDsets))
    , m_WriteInfos(std::move(rvl.m_WriteInfos))
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

  void DescriptorsSetManager::WriteDescsBuilder::processWrite(size_t set, size_t binding,
    vk::DescriptorType dsetType,
    std::function<void(vk::WriteDescriptorSet&)> setupInfoPtr)
  {
    if (!m_DsetManager.validateBinding(set, binding, dsetType))
        return;

    m_DsetManager.insureSetExistance(set);
    m_Writes.updateSets.insert(set);

    vk::WriteDescriptorSet write;
    write.dstSet = m_DsetManager.m_BindedDsets[set];
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorCount = 1;
    write.descriptorType = dsetType;
    setupInfoPtr(write);

    m_Writes.writes.push_back(write);
  }

  void DescriptorsSetManager::WriteDescsBuilder::process(const SamplerWriteInfo& i)
  {
    const auto infoSetuper = [&](vk::WriteDescriptorSet& write) {
      vk::DescriptorImageInfo imgInfo;
      imgInfo.sampler = i.sampler;
      m_Writes.imgInfos.push_back(imgInfo);
      write.pImageInfo = &m_Writes.imgInfos.back();
    };
    processWrite(i.set, i.binding, vk::DescriptorType::eSampler, infoSetuper);
  }

  void DescriptorsSetManager::WriteDescsBuilder::process(const ImageWriteInfo& i)
  {
    const auto infoSetuper = [&](vk::WriteDescriptorSet& write) {
      vk::DescriptorImageInfo imgInfo;
      imgInfo.imageView = i.view;
      imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
      m_Writes.imgInfos.push_back(imgInfo);
      write.pImageInfo = &m_Writes.imgInfos.back();
    };
    processWrite(i.set, i.binding, vk::DescriptorType::eSampledImage, infoSetuper);
  }

  void DescriptorsSetManager::WriteDescsBuilder::process(const UniformBufferWriteInfo& i)
  {
    const auto infoSetuper = [&](vk::WriteDescriptorSet& write) {
        vk::DescriptorBufferInfo bufInfo;
        bufInfo.buffer = i.buffer;
        bufInfo.offset = i.constOffset;
        bufInfo.range = VK_WHOLE_SIZE;
        m_Writes.bufInfos.push_back(bufInfo);
        write.pBufferInfo = &m_Writes.bufInfos.back();
      };
    processWrite(i.set, i.binding, vk::DescriptorType::eUniformBuffer, infoSetuper);
  }


  DescriptorsSetManager::WritesDescription DescriptorsSetManager::acquireWrites()
  {
    WriteDescsBuilder writesBuilder(*this);

    for (const auto& wrInfo: m_WriteInfos)
      std::visit([&](auto& v){
        writesBuilder.process(v);
      }, wrInfo);

    m_WriteInfos.clear();
    return writesBuilder.gatherWrites();
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
