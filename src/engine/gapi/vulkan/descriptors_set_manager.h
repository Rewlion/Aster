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
      bool validateBinding(const size_t set, const size_t binding);

      struct SamplerWriteInfo
      {
        const vk::Sampler sampler;
        const size_t set;
        const size_t binding;
      };
      struct ImageWriteInfo
      {
        const vk::ImageView view;
        const size_t set;
        const size_t binding;
      };
      struct UniformBufferWriteInfo
      {
        const vk::Buffer buffer;
        const size_t constOffset;
        const size_t set;
        const size_t binding;
      };
      typedef std::variant<SamplerWriteInfo, ImageWriteInfo,
                           UniformBufferWriteInfo> WriteInfo;

      bool validateBinding(const size_t set, const size_t binding, const vk::DescriptorType type) const;

      vk::DescriptorSet acquireSet(const size_t set);
      void addPool();
      vk::DescriptorPool& acquirePool();

    private:
      Device& m_Device;
      const PipelineLayout* m_PipelineLayout;
      eastl::vector<vk::UniqueDescriptorPool> m_Pools;
      size_t m_PoolId = 0;

      eastl::vector<WriteInfo> m_WriteInfos;

  };
}
