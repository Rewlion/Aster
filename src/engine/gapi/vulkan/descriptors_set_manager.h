#pragma once

#include "vulkan.h"

#include <shaders_compiler/spirv.h>
#include <engine/utils/bit_capacity.hpp>

#include <EASTL/vector.h>
#include <EASTL/vector_set.h>

#include <functional>
#include <optional>
#include <utility>

namespace gapi::vulkan
{
  class Device;
  struct PipelineLayout;

  class DescriptorsSetManager
  {
    class PoolManager
    {
      public:
        PoolManager(Device&);
        PoolManager(PoolManager&&) = default;
        
        PoolManager& operator=(PoolManager&&) = delete;
        PoolManager& operator=(const PoolManager&) = delete;

        auto acquireSet(const vk::DescriptorSetLayout layout) -> vk::DescriptorSet;

      private:
        void addPool();

      private:
        Device& m_Device;
        eastl::vector<vk::UniqueDescriptorPool> m_Pools;
    };

    struct NullInfo {};
    struct SamplerWriteInfo
    {
      vk::Sampler sampler;
      size_t set;
      size_t binding;
    };

    struct TextureWriteInfo
    {
      TextureHandle texture;
      size_t set;
      size_t binding;
      size_t mip;
    };
  
    struct UniformBufferWriteInfo
    {
      BufferHandler buffer;
      size_t set;
      size_t binding;
    };

    typedef std::variant<SamplerWriteInfo, TextureWriteInfo,
                         UniformBufferWriteInfo, NullInfo> WriteInfo;

    class SetManager
    {
      public:
        SetManager(Device&, PoolManager&, const size_t set);
        SetManager(SetManager&&) = default;
        
        SetManager& operator=(SetManager&&) = delete;
        SetManager& operator=(const SetManager&) = delete;

        void setPipelineLayout(const vk::DescriptorSetLayout, const spirv::v2::DescriptorSet*, const bool acc_toggled = false);

        void setTexture(const TextureHandle, const size_t binding, const size_t mip);
        void setSampler(const vk::Sampler, const size_t binding);
        void setUniformBuffer(const BufferHandler, const size_t binding);

        auto acquireSet(vk::Device) -> vk::DescriptorSet;
        auto isToggled() const -> bool { return m_Toggled; }

      private:
        struct WritePreparation
        {
          eastl::vector<vk::WriteDescriptorSet> writes;
          eastl::vector<vk::DescriptorImageInfo> imgs;
          eastl::vector<vk::DescriptorBufferInfo> bufs;
        };
        void fitWriteInfos(const size_t up_to);
        auto isCompatible(const spirv::v2::DescriptorSet*) const -> bool;
        auto getBindingId(const size_t binding) const -> int;
        auto validateBindingType(const size_t binding, const vk::DescriptorType) const -> bool;
        auto getImageView(const TextureHandle, const size_t binding, const size_t mip) const -> vk::ImageView;
        auto getBufferWithOffset(const BufferHandler) -> std::pair<vk::Buffer, vk::DeviceSize>;

      private:
        Device& m_Device;

        size_t m_SetId;
        PoolManager& m_PoolManager;
        
        bool m_Toggled;
        bool m_Dirty;
        bool m_Active;
        vk::DescriptorSet m_CurrentVkSet;

        eastl::vector<WriteInfo> m_WriteInfos;
        
        vk::DescriptorSetLayout m_Layout;
        const spirv::v2::DescriptorSet* m_Bindings;
    };

    public:
      DescriptorsSetManager(Device& device);
      DescriptorsSetManager(DescriptorsSetManager&& rvl);

      const DescriptorsSetManager& operator=(const DescriptorsSetManager& rvl) = delete;

      void setPipelineLayout(const PipelineLayout* layout, const vk::PipelineBindPoint);

      void setImage(TextureHandle, const size_t set, const size_t binding, const size_t mip);
      void setSampler(const vk::Sampler sampler, const size_t set, const size_t binding);
      void setUniformBuffer(const BufferHandler buffer, const size_t set, const size_t binding);

      void updateDescriptorSets(vk::CommandBuffer& cmdBuf);

    private:
      void fitSetManagers(const size_t up_to);
    
    private:
      Device& m_Device;
      PoolManager m_PoolManager;

      const PipelineLayout* m_PipelineLayout;
      eastl::vector<SetManager> m_SetManagers;

      vk::PipelineBindPoint m_PipelineBindPoint;
  };
}
