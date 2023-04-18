#pragma once

#include "vulkan.h"

#include <shaders_compiler/spirv.h>
#include <engine/utils/bit_capacity.hpp>

#include <EASTL/vector.h>
#include <EASTL/vector_set.h>

#include <optional>
#include <functional>

namespace gapi::vulkan
{
  class Device;
  struct PipelineLayout;

  class DescriptorsSetManager
  {
    class PoolManager
    {
      public:
        PoolManager() = default;
        PoolManager(Device&);
        PoolManager(PoolManager&&) = default;
        PoolManager& operator=(PoolManager&&) = default;

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

    struct ImageWriteInfo
    {
      vk::ImageView view;
      size_t set;
      size_t binding;
    };
  
    struct UniformBufferWriteInfo
    {
      vk::Buffer buffer;
      size_t constOffset;
      size_t set;
      size_t binding;
    };

    typedef std::variant<SamplerWriteInfo, ImageWriteInfo,
                         UniformBufferWriteInfo, NullInfo> WriteInfo;

    class SetManager
    {
      public:
        SetManager(PoolManager&, const size_t set);
        SetManager(SetManager&&) = default;
        SetManager& operator=(SetManager&&) = default;

        void setPipelineLayout(const vk::DescriptorSetLayout, const spirv::v2::DescriptorSet*);

        void setImage(const vk::ImageView, const size_t binding);
        void setSampler(const vk::Sampler, const size_t binding);
        void setUniformBuffer(const vk::Buffer, const size_t binding, const size_t constOffset);

        auto acquireSet(vk::Device) -> vk::DescriptorSet;

      private:
        struct WritePreparation
        {
          eastl::vector<vk::WriteDescriptorSet> writes;
          eastl::vector<vk::DescriptorImageInfo> imgs;
          eastl::vector<vk::DescriptorBufferInfo> bufs;
        };
        auto prepareWrites(const eastl::vector<WriteInfo>&) -> WritePreparation;
        void fitWriteInfos(const size_t up_to);
        auto isCompatible(const spirv::v2::DescriptorSet*) const -> bool;
        auto getBindingId(const size_t binding) const -> int;
        auto validateBindingType(const size_t binding, const vk::DescriptorType) const -> bool;

      private:
        size_t m_SetId;
        PoolManager& m_PoolManager;
        
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

      void setPipelineLayout(const PipelineLayout* layout);

      void setImage(const vk::ImageView imgView, const size_t set, const size_t binding);
      void setSampler(const vk::Sampler sampler, const size_t set, const size_t binding);
      void setUniformBuffer(const vk::Buffer buffer, const size_t set, const size_t binding, const size_t constOffset);

      void updateDescriptorSets(vk::CommandBuffer& cmdBuf);

    private:
      void fitSetManagers(const size_t up_to);
    
    private:
      Device& m_Device;
      PoolManager m_PoolManager;

      const PipelineLayout* m_PipelineLayout;
      eastl::vector<SetManager> m_SetManagers;
  };
}
