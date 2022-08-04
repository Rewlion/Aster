#include "descriptors_set_manager.h"
#include "pipeline_layout.h"
#include "result.h"

#include <engine/log.h>
#include <engine/gapi/vulkan/device.h>

namespace gapi::vulkan
{
  DescriptorsSetManager::DescriptorsSetManager(Device& device)
    : m_Device(device)
    , m_PipelineLayout(nullptr)
    , m_PoolId(0)
  {
  }

  DescriptorsSetManager::DescriptorsSetManager(DescriptorsSetManager&& rvl)
    : m_Device(rvl.m_Device)
    , m_PipelineLayout(rvl.m_PipelineLayout)
    , m_Pools(std::move(rvl.m_Pools))
    , m_PoolId(rvl.m_PoolId)
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
    if ((set < m_PipelineLayout->dsets.size()) &&
        (binding < m_PipelineLayout->dsets[set].size()))
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
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
  }

  void DescriptorsSetManager::updateDescriptorSets(vk::CommandBuffer& cmdBuf)
  {
    ASSERT(m_PipelineLayout != nullptr);
    if (m_WriteInfos.empty())
      return;    

    eastl::vector<vk::DescriptorImageInfo> imgInfos;
    imgInfos.reserve(m_WriteInfos.size());
    eastl::vector<vk::DescriptorBufferInfo> bufInfos;
    bufInfos.reserve(m_WriteInfos.size());

    const auto cbInvoker = [this](const auto& cb, const auto& info, vk::DescriptorType type) {
      if (!validateBinding(info.set, info.binding, type))
            return;
      cb(info, type);
    };

    const auto processSampler = [&](const auto& cb) {
      return [&](const SamplerWriteInfo& info) {
        cbInvoker(cb, info, vk::DescriptorType::eSampler);
      };
    };

    const auto processImage = [&](const auto& cb) {
      return [&](const ImageWriteInfo& info) {
        cbInvoker(cb, info,vk::DescriptorType::eSampledImage);
      };
    };

    const auto processUniformBuffer = [&](const auto& cb) {
      return [&](const UniformBufferWriteInfo& info) {
        cbInvoker(cb, info, vk::DescriptorType::eUniformBuffer);
      };
    };

    for (const auto& wrInfo: m_WriteInfos)
    {
      std::visit(overload{
        processSampler([&](const auto& info, vk::DescriptorType type){
          vk::DescriptorImageInfo imgInfo;
          imgInfo.sampler = info.sampler;
          imgInfos.push_back(imgInfo);
        }),
        processImage([&](const auto& info, vk::DescriptorType type){
          vk::DescriptorImageInfo imgInfo;
          imgInfo.imageView = info.view;
          imgInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
          imgInfos.push_back(imgInfo);
        }),
        processUniformBuffer([&](const auto& info, vk::DescriptorType type){
          vk::DescriptorBufferInfo bufInfo;
          bufInfo.buffer = info.buffer;
          bufInfo.offset = info.constOffset;
          bufInfo.range = VK_WHOLE_SIZE;
          bufInfos.push_back(bufInfo);
        })
      }, wrInfo);
    }

    std::vector<vk::DescriptorSet> updateSets;
    const auto getDset = [&](size_t set) {
      if (updateSets.size() <= set)
        updateSets.resize(set+1);

      vk::DescriptorSet& updatingSet = updateSets[set];

      if (updatingSet == vk::DescriptorSet{})
        updatingSet = acquireSet(set);

      return updatingSet;
    };

    eastl::vector<vk::WriteDescriptorSet> writes;
    writes.reserve(m_WriteInfos.size());
    size_t bufIdx = 0, imgIdx = 0;
    const auto addWrite = [&](const auto& info, vk::DescriptorType type){
      vk::WriteDescriptorSet write;
      write.dstSet = getDset(info.set);
      write.dstBinding = info.binding;
      write.dstArrayElement = 0;
      write.descriptorCount = 1;
      write.descriptorType = type;

      if (type == vk::DescriptorType::eUniformBuffer)
        write.pBufferInfo = &bufInfos[bufIdx++];
      else
        write.pImageInfo = &imgInfos[imgIdx++];

      writes.push_back(write);
    };

    for (const auto& wrInfo: m_WriteInfos)
    {
      std::visit(overload{
        processSampler(addWrite),
        processImage(addWrite),
        processUniformBuffer(addWrite)
      }, wrInfo);
    }

    m_WriteInfos.clear();

    if (!writes.empty())
    {
      m_Device.getDevice().updateDescriptorSets(writes.size(), writes.data(), 0, nullptr);
      for (size_t set = 0; set < updateSets.size(); ++set)
      {
        if (updateSets[set] != vk::DescriptorSet{})
        {
          cmdBuf.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                    m_PipelineLayout->pipelineLayout.get(),
                                    set, 1, &updateSets[set], 0, nullptr);
        }
      }
    }
  }

  void DescriptorsSetManager::setPipelineLayout(const PipelineLayout* layout)
  {
    ASSERT(layout != nullptr);
    m_PipelineLayout = layout;
  }
}
