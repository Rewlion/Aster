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
      struct WritesDescription
      {
        eastl::vector<vk::DescriptorImageInfo> imgInfos;
        eastl::vector<vk::DescriptorBufferInfo> bufInfos;
        eastl::vector<vk::WriteDescriptorSet> writes;
        eastl::vector_set<size_t> updateSets;
      };
      friend class WriteDescsBuilder;
      class WriteDescsBuilder
      {
        public:
          WriteDescsBuilder(DescriptorsSetManager& dsetManager)
            : m_DsetManager(dsetManager)
          {
          }
          void process(const SamplerWriteInfo& i);
          void process(const ImageWriteInfo& i);
          void process(const UniformBufferWriteInfo& i);

          inline WritesDescription&& gatherWrites() { return std::move(m_Writes); }
        private:
          void processWrite(size_t set, size_t binding,
                            vk::DescriptorType dsetType,
                            std::function<void(vk::WriteDescriptorSet&)> setupInfoPtr);
        private:
          DescriptorsSetManager& m_DsetManager;
          WritesDescription m_Writes;
      };

      typedef std::variant<SamplerWriteInfo, ImageWriteInfo,
                           UniformBufferWriteInfo> WriteInfo;

      bool validateBinding(const size_t set, const size_t binding, const vk::DescriptorType type) const;
      void insureSetExistance(const size_t set);
      WritesDescription acquireWrites();

      vk::DescriptorSet acquireSet(const size_t set);
      void addPool();
      vk::DescriptorPool& acquirePool();

    private:
      Device& m_Device;
      const PipelineLayout* m_PipelineLayout;
      eastl::vector<vk::UniqueDescriptorPool> m_Pools;
      size_t m_PoolId = 0;

      eastl::vector<vk::DescriptorSet> m_BindedDsets;
      eastl::vector<WriteInfo> m_WriteInfos;

  };
}
