#include "descriptors_set_manager.h"

#include "compiler_to_vk.h"
#include "gapi_to_vk.h"
#include "pipeline_layout.h"
#include "result.h"

#include <engine/gapi/vulkan/device.h>
#include <engine/log.h>
#include <engine/utils/pattern_matching.hpp>

#include <shaders_compiler/limits.h>

#include <ranges>

namespace gapi::vulkan
{
  DescriptorsSetManager::PoolManager::PoolManager(Device& device)
    : m_Device(device)
  {
    addPool();
  }

  void DescriptorsSetManager::PoolManager::addPool()
  {
    vk::DescriptorPoolSize sizes[] = {
      vk::DescriptorPoolSize(vk::DescriptorType::eSampler, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eCombinedImageSampler, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eSampledImage, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eStorageImage, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eUniformTexelBuffer, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eStorageTexelBuffer, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eStorageBuffer, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eUniformBufferDynamic, 100),
      vk::DescriptorPoolSize(vk::DescriptorType::eStorageBufferDynamic, 100)
    };

    vk::DescriptorPoolCreateInfo ci{};
    ci.maxSets = 1024;
    ci.pPoolSizes = sizes;
    ci.poolSizeCount = std::size(sizes);

    auto dsPoolUnique = m_Device.getDevice().createDescriptorPoolUnique(ci);
    VK_CHECK_RES(dsPoolUnique);

    m_Pools.push_back(std::move(dsPoolUnique.value));
  }

  auto DescriptorsSetManager::PoolManager::acquireSet(const vk::DescriptorSetLayout layout,
                                                      const bool has_variable_sized_binding) -> vk::DescriptorSet
  {
    const auto allocSet = [has_variable_sized_binding](auto& device, auto pool, auto& layout)
    {
      const uint maxVariableSizedBinding = gapi::MAX_VARIABLE_ARRAY_SIZE-1;
      vk::DescriptorSetVariableDescriptorCountAllocateInfo vdcAllocInfo;
      vdcAllocInfo.descriptorSetCount = 1;
      vdcAllocInfo.pDescriptorCounts = &maxVariableSizedBinding;

      vk::DescriptorSetAllocateInfo dsAi;
      dsAi.descriptorPool = pool;
      dsAi.descriptorSetCount = 1;
      dsAi.pSetLayouts = &layout;

      if (has_variable_sized_binding)
        dsAi.pNext = &vdcAllocInfo;

      return device.allocateDescriptorSets(dsAi);
    };

    const auto res = allocSet(m_Device.getDevice(), m_Pools.back().get(), layout);
    if (res.result != vk::Result::eSuccess) [[unlikely]]
    {
      addPool();
      const auto res = allocSet(m_Device.getDevice(), m_Pools.back().get(), layout);;
      VK_CHECK_RES(res);
    }

    return res.value[0];
  }

  void DescriptorsSetManager::VariableSizeBinding::setSampler(
                                                    const vk::Sampler sampler,
                                                    const size_t set,
                                                    const size_t binding,
                                                    const size_t dst_array_element)
  {
    switchMode(Mode::Sampler);
    fitWriteInfos(binding);

    m_DstElemsWriteInfos[dst_array_element] = SamplerWriteInfo{
      .sampler = sampler,
      .set = set,
      .binding = binding,
      .dstArrayElement = dst_array_element,
    };

  }

  void DescriptorsSetManager::VariableSizeBinding::setUniformBuffer(
                                                    const BufferHandler buffer,
                                                    const size_t set,
                                                    const size_t binding,
                                                    const size_t dst_array_element)
  {
    switchMode(Mode::Buffer);
    fitWriteInfos(binding);

    m_DstElemsWriteInfos[dst_array_element] = UniformBufferWriteInfo{
      .buffer = buffer,
      .set = set,
      .binding = binding,
      .dstArrayElement = dst_array_element,
    };
  }

  void DescriptorsSetManager::VariableSizeBinding::setTexture(
                                                     const TextureHandle tex,
                                                     const size_t set,
                                                     const size_t binding,
                                                     const size_t dst_array_element,
                                                     const size_t mip)
  {
    switchMode(Mode::Texture);
    fitWriteInfos(dst_array_element);

    m_DstElemsWriteInfos[dst_array_element] = TextureWriteInfo{
      .texture = tex,
      .set = set,
      .binding = binding,
      .dstArrayElement = dst_array_element,
      .mip = mip
    };
  }

  void DescriptorsSetManager::VariableSizeBinding::switchMode(const Mode mode)
  {
    if (m_Mode != mode)
    {
      m_DstElemsWriteInfos.clear();
      m_Mode = mode;
      m_ElemsRangeEnd = 0;
    }
  }

  void DescriptorsSetManager::VariableSizeBinding::fitWriteInfos(const size_t dst_array_element)
  {
    m_DstElemsWriteInfos.resize(dst_array_element+1);
    m_ElemsRangeEnd = std::max(dst_array_element+1, m_ElemsRangeEnd);
  }

  DescriptorsSetManager::SetManager::SetManager(Device& device,
                                                PoolManager& pool, 
                                                const size_t set)
    : m_Device(device)
    , m_SetId(set)
    , m_PoolManager(pool)
    , m_Toggled(false)
    , m_Dirty(false)
    , m_Active(false)
    , m_Bindings(nullptr)
  {
  }

  void DescriptorsSetManager::SetManager::fitWriteInfos(const size_t up_to)
  {
    for (size_t i = m_WriteInfos.size(); i <= up_to; ++i)
      m_WriteInfos.push_back(NullInfo{});
  }

  auto DescriptorsSetManager::SetManager::isCompatible(const spirv::v2::DescriptorSet* bindings) const -> bool
  {
    if (m_Bindings && (bindings->size() <= m_Bindings->size()))
      return !std::memcmp(m_Bindings->data(), bindings->data(), bindings->size() * sizeof(bindings[0]));

    return false;
  }

  auto DescriptorsSetManager::SetManager::getBindingId(const size_t binding) const -> int
  {
    const size_t total = m_Bindings->size();
    for (size_t i = 0; i < total; ++i)
      if ((*m_Bindings)[i].vk.binding == binding)
        return i;
    
    return -1;
  }

  auto DescriptorsSetManager::SetManager::validateBindingType(const size_t binding,
                                                              const vk::DescriptorType type) const -> bool
  {
    const int id = getBindingId(binding);
    return (id >= 0) && (*m_Bindings)[id].vk.descriptorType == type;
  }

  void DescriptorsSetManager::SetManager::setPipelineLayout(const vk::DescriptorSetLayout layout,
                                                            const spirv::v2::DescriptorSet* bindings,
                                                            const bool has_variable_sized_binding,
                                                            const bool acc_toggled)
  {
    m_Layout = layout;

    const bool oldActive = m_Active;
    m_Active = bindings && !bindings->empty();
    if (!m_Active)
      return;

    m_Toggled = acc_toggled || (!oldActive && m_Active);
    m_Dirty |= !isCompatible(bindings);

    m_Bindings = bindings;
    m_HasVariableSizedBinding = has_variable_sized_binding;
  }

  void DescriptorsSetManager::SetManager::setTexture(const TextureHandle tex,
                                                     const size_t binding,
                                                     const size_t dst_array_element,
                                                     const size_t mip)
  {
    if (binding != VARIABLE_SIZE_ARRAY_REGISTER) [[likely]]
    {
      fitWriteInfos(binding);
      m_WriteInfos[binding] = TextureWriteInfo{
        .texture = tex,
        .set = m_SetId,
        .binding = binding,
        .dstArrayElement = dst_array_element,
        .mip = mip
      };
    }
    else
      m_VariableSizeBinding.setTexture(tex, m_SetId, binding, dst_array_element, mip);

    m_Dirty = true;
  }

  void DescriptorsSetManager::SetManager::setSampler(const vk::Sampler sampler,
                                                     const size_t binding,
                                                     const size_t dst_array_element)
  {
    if (binding != VARIABLE_SIZE_ARRAY_REGISTER) [[likely]]
    {
      fitWriteInfos(binding);
      m_WriteInfos[binding] = SamplerWriteInfo{
        .sampler = sampler,
        .set = m_SetId,
        .binding = binding,
        .dstArrayElement = dst_array_element,
      };
    }
    else
      m_VariableSizeBinding.setSampler(sampler, m_SetId, binding, dst_array_element);

    m_Dirty = true;
  }

  void DescriptorsSetManager::SetManager::setUniformBuffer(const BufferHandler buffer,
                                                           const size_t binding,
                                                           const size_t dst_array_element)
  {
    if (binding != VARIABLE_SIZE_ARRAY_REGISTER) [[likely]]
    {
      fitWriteInfos(binding);
      m_WriteInfos[binding] = UniformBufferWriteInfo{
        .buffer = buffer,
        .set = m_SetId,
        .binding = binding,
        .dstArrayElement = dst_array_element,
      };
    }
    else
      m_VariableSizeBinding.setUniformBuffer(buffer, m_SetId, binding, dst_array_element);
    
    m_Dirty = true;
  }

  namespace
  {
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    template<class... Ts> overload(Ts...) -> overload<Ts...>;
  }

  extern auto get_resource_stub(ShadersSystem::ResourceType type) -> ResourceHandler;

  auto DescriptorsSetManager::SetManager::getImageView(const TextureHandle handle,
                                                       const size_t binding,
                                                       const size_t mip) const -> vk::ImageView
  {
    const size_t bindingId = getBindingId(binding);
    const ShadersSystem::ResourceType expectedResourceType = (*m_Bindings)[bindingId].resourceType;
    if (handle != TextureHandle::Invalid)
    {
      const vk::ImageViewType viewType = m_Device.getImageViewType(handle);
      const vk::ImageViewType expectedViewType = shaders_resource_to_img_view_type(expectedResourceType);
      if (viewType == expectedViewType)
        return m_Device.getImageView(handle, true, mip);
    }

    return m_Device.getImageView((TextureHandle)get_resource_stub(expectedResourceType), true);
  }

  auto DescriptorsSetManager::SetManager::getBufferWithOffset(const BufferHandler handle) -> std::pair<vk::Buffer, vk::DeviceSize>
  {
    const BufferHandler h = handle != gapi::BufferHandler::Invalid ?
                              handle :
                              (gapi::BufferHandler)get_resource_stub(ShadersSystem::ResourceType::Buffer);

    const Buffer& buffer = m_Device.getBuffer(h);
    return {
      buffer.buffer,
      0
    };
  }

  auto DescriptorsSetManager::SetManager::acquireSet(vk::Device device) -> vk::DescriptorSet
  {
    if (!m_Active)
      return {};

    if (m_Dirty)
    {
      const auto allWriteInfos = {
        eastl::span<const WriteInfo>(m_WriteInfos),
        m_VariableSizeBinding.getWriteInfos()
      };
      const auto allWriteInfosView = std::ranges::join_view(allWriteInfos);

      size_t imgCount = 0;
      size_t bufCount = 0;
      for (const auto& wrInfo: allWriteInfosView)
      {
        wrInfo >> match {
          [&](const SamplerWriteInfo& info){imgCount++;},
          [&](const TextureWriteInfo& info){imgCount++;},
          [&](const UniformBufferWriteInfo& info){bufCount++;},
          [&](const NullInfo&){}
        };
      }

      eastl::vector<vk::WriteDescriptorSet> writes;
      eastl::vector<vk::DescriptorImageInfo> imgInfos;
      eastl::vector<vk::DescriptorBufferInfo> bufInfos;

      writes.reserve(m_WriteInfos.size());
      imgInfos.resize(imgCount);
      bufInfos.resize(bufCount);

      m_CurrentVkSet = m_PoolManager.acquireSet(m_Layout, m_HasVariableSizedBinding);

      const auto addWriteInfo = [&writes, this](const size_t binding,
                                                const size_t dst_array_element,
                                                const vk::DescriptorType type,
                                                const vk::DescriptorBufferInfo* buf_info,
                                                const vk::DescriptorImageInfo* img_info)
      {
        auto& write = writes.push_back();
        write.dstSet = m_CurrentVkSet;
        write.dstBinding = binding;
        write.dstArrayElement = dst_array_element;
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pBufferInfo = buf_info;
        write.pImageInfo = img_info;
      };

      size_t iImg = 0, iBuf = 0;
      for (const auto& wrInfo: allWriteInfosView)
      {
        wrInfo >> match {
          [&](const SamplerWriteInfo& info){
            auto type = vk::DescriptorType::eSampler;
            if (validateBindingType(info.binding, type))
            {
              imgInfos[iImg].sampler = info.sampler;
              addWriteInfo(info.binding, info.dstArrayElement, type, nullptr, &imgInfos[iImg]);
              ++iImg;
            }
          },

          [&](const TextureWriteInfo& info){
            auto addWrite = [&](const vk::DescriptorType type, vk::ImageLayout layout)
            {
              if(validateBindingType(info.binding, type))
              {
                imgInfos[iImg].imageView = getImageView(info.texture, info.binding, info.mip);
                imgInfos[iImg].imageLayout = layout;
                addWriteInfo(info.binding, info.dstArrayElement, type, nullptr, &imgInfos[iImg]);
                ++iImg;
                return true;
              }
              return false;
            };

            const bool isDepth = (info.texture != gapi::TextureHandle::Invalid) && is_depth_format(m_Device.getTextureFormat(info.texture));

            if (!(isDepth && addWrite(vk::DescriptorType::eSampledImage, vk::ImageLayout::eDepthStencilReadOnlyOptimal)))
              if (!addWrite(vk::DescriptorType::eSampledImage, vk::ImageLayout::eShaderReadOnlyOptimal))
                addWrite(vk::DescriptorType::eStorageImage, vk::ImageLayout::eGeneral);
          },

          [&](const UniformBufferWriteInfo& info){
            auto addWrite = [&](const vk::DescriptorType type)
            {
              if(validateBindingType(info.binding, type))
              {
                const auto [buf, offset] = getBufferWithOffset(info.buffer);
                bufInfos[iBuf].buffer = buf;
                bufInfos[iBuf].offset = offset;
                bufInfos[iBuf].range = VK_WHOLE_SIZE;
                addWriteInfo(info.binding, info.dstArrayElement, type, &bufInfos[iBuf], nullptr);
                ++iBuf;
                return true;
              }
              return false;
            };

            if (!addWrite(vk::DescriptorType::eUniformBuffer))
              //if (!addWrite(vk::DescriptorType::eUniformTexelBuffer)) //requires buffer view  T___T
                addWrite(vk::DescriptorType::eStorageBuffer);
          },

          [&](const NullInfo&) {}
        };
      }

      device.updateDescriptorSets(writes.size(), writes.data(), 0, nullptr);
    }

    if (m_Toggled || m_Dirty)
    {
      m_Toggled = false;
      m_Dirty = false;
      return m_CurrentVkSet;
    }

    return {};
  }


  DescriptorsSetManager::DescriptorsSetManager(Device& device)
    : m_Device(device)
    , m_PoolManager(device)
    , m_PipelineLayout(nullptr)
    , m_PipelineBindPoint(vk::PipelineBindPoint::eGraphics)
  {
  }

  DescriptorsSetManager::DescriptorsSetManager(DescriptorsSetManager&& rvl)
    : m_Device(rvl.m_Device)
    , m_PoolManager(std::move(rvl.m_PoolManager))
    , m_PipelineLayout(rvl.m_PipelineLayout)
    , m_PipelineBindPoint(rvl.m_PipelineBindPoint)
  {
  }

  void DescriptorsSetManager::setImage(TextureHandle tex, const size_t set, const size_t binding, const size_t dst_array_element, const size_t mip)
  {
    fitSetManagers(set);
    m_SetManagers[set].setTexture(tex, binding, dst_array_element, mip);
  }

  void DescriptorsSetManager::setSampler(const vk::Sampler sampler, const size_t set, const size_t binding, const size_t dst_array_element)
  {
    fitSetManagers(set);
    m_SetManagers[set].setSampler(sampler, binding, dst_array_element);
  }

  void DescriptorsSetManager::setUniformBuffer(const BufferHandler buffer, const size_t set, const size_t binding, const size_t dst_array_element)
  {
    fitSetManagers(set);
    m_SetManagers[set].setUniformBuffer(buffer, binding, dst_array_element);
  }

  void DescriptorsSetManager::updateDescriptorSets(vk::CommandBuffer& cmdBuf)
  {
    ASSERT(m_PipelineLayout != nullptr);
    
    vk::Device device = m_Device.getDevice();
    const size_t setsCount = m_PipelineLayout->dsets.size();
    for (size_t i = 0; i < setsCount; ++i)
    {
      vk::DescriptorSet vkSet = m_SetManagers[i].acquireSet(device);
      if (vkSet != vk::DescriptorSet{})
      {
        cmdBuf.bindDescriptorSets(m_PipelineBindPoint,
                                  m_PipelineLayout->pipelineLayout.get(),
                                  i, 1, &vkSet, 0, nullptr);
      }
    }
  }

  void DescriptorsSetManager::setPipelineLayout(const PipelineLayout* layout, const vk::PipelineBindPoint bind_point)
  {
    ASSERT(layout != nullptr);
    m_PipelineBindPoint = bind_point;
    m_PipelineLayout = layout;

    const size_t activeSetsCount = layout->dsets.size();
    const size_t managersCount = m_SetManagers.size();
    const size_t restSets = managersCount > activeSetsCount ? managersCount - activeSetsCount : 0; 
    
    if (activeSetsCount > 0)
      fitSetManagers(activeSetsCount-1);

    bool accumulatedToggling = false;
    for (size_t i = 0; i < activeSetsCount; ++i)
    {
      m_SetManagers[i].setPipelineLayout(layout->descriptorSetLayouts[i].get(), &layout->dsets[i], layout->variableSizeDsets.isSet(i), accumulatedToggling);
      accumulatedToggling |= m_SetManagers[i].isToggled();
    }

    for (size_t i = activeSetsCount; i < restSets; ++i)
      m_SetManagers.back().setPipelineLayout({}, nullptr);
  }

  void DescriptorsSetManager::fitSetManagers(const size_t up_to)
  {
    for (size_t i = m_SetManagers.size(); i <= up_to; ++i)
      m_SetManagers.emplace_back(m_Device, m_PoolManager, i);
  }
}
