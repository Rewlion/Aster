#pragma once

#include <vulkan/vulkan.hpp>
#include "Shader.h"
#include "buffer.h"

namespace Vulkan
{
  class Core;
  class Image;
  class ImageView;

  class UniformsAccessor
  {
  public:
    UniformsAccessor(Core& core, vk::DescriptorPool descriptorPool, const std::vector<vk::DescriptorSetLayout>& layouts, const PipelineUniforms& uniforms);

    template<class T>
    void SetUniformBuffer(const UniformName& name, const T* data)
    {
      auto [setBinding, bindingDescription, dscSet] = AccessDescriptorSet(name, UniformType::UniformBuffer);

      if (sizeof(T) != bindingDescription.size)
        throw std::runtime_error("UniformsAccessor::GetUniformBuffer, uniform's size is not equal to the requested mapping structure.");

      HostBuffer buf = core.AllocateHostBuffer(bindingDescription.size, vk::BufferUsageFlagBits::eUniformBuffer);
      buf.UploadMemory(data, sizeof(T), 0);
      ownedBuffers.push_back(std::move(buf));

      writes[setBinding] = vk::WriteDescriptorSet()
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setDstArrayElement(0)
        .setDstBinding(setBinding.binding)
        .setDstSet(dscSet)
        .setPBufferInfo(&ownedBuffers.back().GetFullBufferUpdateInfo());
    }

    std::tuple<UniformSetPair, UniformBindingDescription, vk::DescriptorSet> AccessDescriptorSet(const UniformName& name, UniformType type);

    void SetSampler2D(const UniformName& name, const Image& img);

    void SetSamplerCube(const UniformName& name, const ImageView& img);

    void SetSubpassInput(const UniformName& name, const ImageView& img);

    std::vector<vk::DescriptorSet> GetUpdatedDescriptorSets();

  private:
    Core& core;
    vk::DescriptorPool descriptorPool;
    std::vector<vk::DescriptorSetLayout> layouts;
    PipelineUniforms uniforms;

    std::vector<vk::DescriptorSet> currentDescriptorSets;
    std::vector<vk::UniqueDescriptorSet> ownedDescriptorSets;
    std::vector<Buffer> ownedBuffers;

    std::map<UniformSetPair, vk::WriteDescriptorSet> writes;
  };
}
