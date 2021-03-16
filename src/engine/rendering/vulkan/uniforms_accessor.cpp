#include "uniforms_accessor.h"
#include "core.h"
#include "image.h"

namespace Vulkan
{
  UniformsAccessor::UniformsAccessor(Core& core, vk::DescriptorPool descriptorPool, const std::vector<vk::DescriptorSetLayout>& layouts, const PipelineUniforms& uniforms)
    : core(core)
    , descriptorPool(descriptorPool)
    , layouts(layouts)
    , uniforms(uniforms)
  {
    currentDescriptorSets.resize(layouts.size());
  }

  std::tuple<UniformSetPair, UniformBindingDescription, vk::DescriptorSet> UniformsAccessor::AccessDescriptorSet(const UniformName& name, UniformType type)
  {
    const UniformSetPair setBinding = uniforms.GetSetBindingPair(name);
    const UniformBindingDescription& bindingDescription = uniforms.GetBindingDescription(setBinding.set, setBinding.binding);

    if (bindingDescription.type != type)
      throw std::runtime_error("UniformsAccessor::AccessDescriptorSet, uniform doesn't have a required type.");

    const bool isBindingAlreadyInUse = writes.find(setBinding) != writes.end();
    vk::DescriptorSet& dscSet = currentDescriptorSets[setBinding.set];

    if (dscSet == vk::DescriptorSet{} || isBindingAlreadyInUse)
    {
      const auto allocInfo = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(descriptorPool)
        .setDescriptorSetCount(1)
        .setPSetLayouts(&layouts[setBinding.set]);

      ownedDescriptorSets.push_back(
        std::move(core.GetLogicalDevice().allocateDescriptorSetsUnique(allocInfo)[0])
      );

      dscSet = ownedDescriptorSets.back().get();

      for (auto& [_, write] : writes)
      {
        write.dstSet = dscSet;
      }
    }

    return { setBinding, bindingDescription, dscSet };
  }

  void UniformsAccessor::SetSampler2D(const UniformName& name, const Image& img)
  {
    auto [setBinding, _, dscSet] = AccessDescriptorSet(name, UniformType::Sampler2D);

    writes[setBinding] = vk::WriteDescriptorSet()
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
      .setDstArrayElement(0)
      .setDstBinding(setBinding.binding)
      .setDstSet(dscSet)
      .setPImageInfo(&img.GetDescriptorImageInfo());
  }

  void UniformsAccessor::SetSamplerCube(const UniformName& name, const ImageView& img)
  {
    auto [setBinding, _, dscSet] = AccessDescriptorSet(name, UniformType::SamplerCube);

    writes[setBinding] = vk::WriteDescriptorSet()
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
      .setDstArrayElement(0)
      .setDstBinding(setBinding.binding)
      .setDstSet(dscSet)
      .setPImageInfo(&img.GetDescriptorImageInfo());
  }

  void UniformsAccessor::SetSubpassInput(const UniformName& name, const ImageView& view)
  {
    auto [setBinding, _, dscSet] = AccessDescriptorSet(name, UniformType::SubpassInput);

    writes[setBinding] = vk::WriteDescriptorSet()
      .setDescriptorCount(1)
      .setDescriptorType(vk::DescriptorType::eInputAttachment)
      .setDstArrayElement(0)
      .setDstBinding(setBinding.binding)
      .setDstSet(dscSet)
      .setPImageInfo(&view.GetDescriptorImageInfo());
  }

  std::vector<vk::DescriptorSet> UniformsAccessor::GetUpdatedDescriptorSets()
  {
    if (writes.size() > 0)
    {
      std::vector<vk::WriteDescriptorSet> writesInfo;
      writesInfo.reserve(writes.size());

      for (const auto& [_, w] : writes)
        writesInfo.push_back(w);

      core.GetLogicalDevice().updateDescriptorSets(writesInfo.size(), writesInfo.data(), 0, nullptr);
    }

    return currentDescriptorSets;
  }
}