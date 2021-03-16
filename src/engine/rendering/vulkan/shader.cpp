#include "Shader.h"
#include "core.h"

#include <engine/utils/utils.h>

#include <iostream>

namespace
{
  vk::DescriptorType GetDescriptorType(Vulkan::UniformType type)
  {
    switch (type)
    {
    case Vulkan::UniformType::UniformBuffer:
      return vk::DescriptorType::eUniformBuffer;

    case Vulkan::UniformType::SamplerCube:
    case Vulkan::UniformType::Sampler2D:
      return vk::DescriptorType::eCombinedImageSampler;

    case Vulkan::UniformType::SubpassInput:
      return vk::DescriptorType::eInputAttachment;

    default:
      throw std::runtime_error("unknown uniform type.");
    }
  }

  vk::ShaderStageFlags GetShaderStageFlag(Vulkan::ShaderStages stages)
  {
    vk::ShaderStageFlags bits;

    if (HAS_STAGE(stages, SHADER_VERTEX_STAGE))
      bits |= vk::ShaderStageFlagBits::eVertex;

    if (HAS_STAGE(stages, SHADER_FRAGMENT_STAGE))
      bits |= vk::ShaderStageFlagBits::eFragment;

    return bits;
  }
}

namespace Vulkan
{
  Shader::Shader(vk::Device logicalDevice, const std::vector<uint32_t>& byteCode)
  {
    uniforms = SpirvParser().ParseShader(byteCode);

    const auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo()
      .setCodeSize(byteCode.size() * sizeof(uint32_t))
      .setPCode(byteCode.data());

    shaderModule = logicalDevice.createShaderModuleUnique(shaderModuleCreateInfo);
  }

  vk::ShaderModule Shader::GetModule() const
  {
    return shaderModule.get();
  }

  ShaderProgram::ShaderProgram(Core& core, Shader&& v, Shader&& fr)
    : core(core)
    , vertex(std::move(v))
    , fragment(std::move(fr))
  {
    id = Utils::UUID();
    uniforms = vertex.GetUniformsDescriptions() + fragment.GetUniformsDescriptions();
    layouts = CreateLayouts(core, uniforms);
  }

  ShaderProgram::~ShaderProgram()
  {
    for (const vk::DescriptorSetLayout& layout : layouts)
    {
      core.GetLogicalDevice().destroyDescriptorSetLayout(layout);
    }
  }

  std::vector<vk::DescriptorSetLayout> ShaderProgram::CreateLayouts(Core& core, const PipelineUniforms& uniforms) const
  {
    std::vector<vk::DescriptorSetLayout> layouts;

    layouts.reserve(uniforms.sets.size());

    for (int i = 0; i < uniforms.sets.size(); ++i)
    {
      const UniformSetDescription& set = uniforms.sets[i];

      std::vector<vk::DescriptorSetLayoutBinding> bindings;
      for (int j = 0; j < set.bindings.size(); ++j)
      {
        const UniformBindingDescription& binding = set.bindings[j];

        if (binding.type == UniformType::None)
          continue;

        const auto bindingDescription = vk::DescriptorSetLayoutBinding()
          .setBinding(j)
          .setDescriptorCount(1)
          .setDescriptorType(GetDescriptorType(binding.type))
          .setStageFlags(GetShaderStageFlag(binding.stages));

        bindings.push_back(bindingDescription);
      }

      const auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
        .setBindingCount(bindings.size())
        .setPBindings(bindings.data());
      //.setFlags()

      vk::DescriptorSetLayout layout = core.GetLogicalDevice().createDescriptorSetLayout(layoutCreateInfo);
      layouts.push_back(layout);
    }

    return layouts;
  }
}