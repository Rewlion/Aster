#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS 1

#include <engine/shaders_compiler/spirv.h>
#include <engine/utils/bit_capacity.hpp>

#include <vulkan/vulkan.hpp>
#include <EASTL/vector.h>

#include <optional>

namespace gapi::vulkan
{
  class Device;
  struct PipelineLayout;

  class DescriptorsSetManager
  {
    public:
      void init(Device* device);

      void setPipelineLayout(const PipelineLayout* layout);

      void setImage(const vk::ImageView imgView, const size_t set, const size_t binding);
      void setSampler(const vk::Sampler sampler, const size_t set, const size_t binding);

      void updateDescriptorSets(vk::CommandBuffer& cmdBuf);

      void reset();
    private:
      bool validateBinding(const size_t set, const size_t binding);

      vk::DescriptorSet acquireSet(const size_t set);

      void addPool();

      vk::DescriptorPool& acquirePool();

      struct Binding
      {
        std::optional<vk::DescriptorType> type;

        vk::ImageView imgView;
        vk::Sampler sampler;
        vk::DescriptorImageInfo imgInfo;
      };

      struct Set
      {
        Binding bindings[spirv::MAX_BINDING_COUNT];

        inline size_t getBindingsCount() const
        {
          size_t size = 0;
          for (size_t i = 0; i < spirv::MAX_BINDING_COUNT; ++i)
            if (bindings[i].type.has_value())
              size = i + 1;
          return size;
        }
      };

      vk::WriteDescriptorSet acquireWriteDescriptorSet(const size_t nSet, const size_t nBinding);

    private:
      Device* m_Device = nullptr;
      const PipelineLayout* m_PipelineLayout;
      eastl::vector<vk::UniqueDescriptorPool> m_Pools;
      size_t m_PoolId = 0;

      Utils::BitCapacity<spirv::MAX_SETS_COUNT> m_DirtySets;
      Utils::BitCapacity<spirv::MAX_SETS_COUNT> m_ActiveSets;
      Set m_Sets[spirv::MAX_SETS_COUNT];

      vk::DescriptorSet m_ActiveDescriptorSets[spirv::MAX_SETS_COUNT];
  };
}
