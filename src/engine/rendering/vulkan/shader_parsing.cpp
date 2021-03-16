#include "shader_parsing.h"

#include <iostream>

#include <spirv_cross/spirv_glsl.hpp>

namespace
{
  Vulkan::ShaderStages GetShaderStage(const spirv_cross::EntryPoint& ep)
  {
    switch (ep.execution_model)
    {
    case spv::ExecutionModel::ExecutionModelVertex:
      return SHADER_VERTEX_STAGE;

    case spv::ExecutionModel::ExecutionModelFragment:
      return SHADER_FRAGMENT_STAGE;

    default:
      const std::string err = std::string("GetShaderStage: unknown shader stage: ") + std::to_string(ep.execution_model);
      throw std::runtime_error(err.c_str());
    }
  }

  Vulkan::UniformType GetUniformTypeFromSpirvDim(spv::Dim dim)
  {
    switch (dim)
    {
    case spv::Dim::Dim2D:
      return Vulkan::UniformType::Sampler2D;

    case spv::Dim::DimCube:
      return Vulkan::UniformType::SamplerCube;

    default:
      throw std::runtime_error("unsupported dim for sampler");
    }
  }
}

namespace Vulkan
{
  void PipelineUniforms::AddUniform(unsigned int set, unsigned int binding, const std::string& name, const UniformBindingDescription& description)
  {
    if (set >= sets.size())
    {
      sets.resize(set + 1);
    }

    UniformSetDescription& setRef = sets.at(set);
    setRef.inUse = true;

    if (binding >= setRef.bindings.size())
    {
      setRef.bindings.resize(binding + 1);
    }

    UniformBindingDescription& bindingRef = setRef.bindings[binding];
    if (bindingRef.type != UniformType::None)
      throw std::runtime_error("Can't add new uniform binding: binding already in use.");

    bindingRef = description;
    uniformsMap[name] = UniformSetPair{ set, binding };
  }

  const UniformBindingDescription& PipelineUniforms::GetBindingDescription(const unsigned int set, const unsigned int binding) const
  {
    return sets.at(set).bindings.at(binding);
  }

  const UniformBindingDescription& PipelineUniforms::GetBindingDescription(const UniformName& name) const
  {
    const UniformSetPair& setBinding = uniformsMap.at(name);
    return GetBindingDescription(setBinding.set, setBinding.binding);
  }

  const UniformSetPair& PipelineUniforms::GetSetBindingPair(const UniformName& name) const
  {
    return uniformsMap.at(name);
  }

  PipelineUniforms operator+(const PipelineUniforms& l, const PipelineUniforms& r)
  {
    PipelineUniforms base = l;
    std::vector<UniformSetDescription>& baseSets = base.sets;
    const std::vector<UniformSetDescription>& mergeSets = r.sets;

    const size_t minSetsSize = baseSets.size() > mergeSets.size() ? mergeSets.size() : baseSets.size();
    //merge sets
    for (size_t i = 0; i < minSetsSize; ++i)
    {
      if (mergeSets[i].inUse == false)
        continue;

      std::vector<UniformBindingDescription>& baseBindings = baseSets[i].bindings;
      const std::vector<UniformBindingDescription>& mergeBindings = mergeSets[i].bindings;

      const size_t minBindingsSize = baseBindings.size() > mergeBindings.size() ? mergeBindings.size() : baseBindings.size();
      //merge bindings from r to l
      //if l's binding is not in use and r's in use than just copy the binding
      //if l's binding is in use:
      //             check if r's is in use. If it's not in use - ignore
      //                   if r's is in use it must be equal to l's despite of stage!
      for (size_t j = 0; j < minBindingsSize; ++j)
      {
        UniformBindingDescription& baseBinding = baseBindings[j];
        const UniformBindingDescription& mergeBinding = mergeBindings[j];

        if (baseBinding.type == UniformType::None && mergeBinding.type != UniformType::None)
        {
          baseBinding = mergeBinding;
          continue;
        }
        else
        {
          if (mergeBinding.type != UniformType::None)
          {
            if (baseBinding.IsEqualWithoutStages(mergeBinding))
              throw std::runtime_error("PipelineUniforms::operator+: different uniforms in the same [set:binding].");

            //it is the same uniform in different pipeline stages
            baseBinding.stages |= mergeBinding.stages;
          }
        }
      }

      if (mergeBindings.size() > baseBindings.size())
      {
        for (size_t j = minBindingsSize; j < mergeBindings.size(); ++j)
          baseBindings.push_back(mergeBindings[j]);
      }
    }

    if (mergeSets.size() > baseSets.size())
    {
      for (size_t i = minSetsSize; i < mergeSets.size(); ++i)
        baseSets.push_back(mergeSets[i]);
    }

    //merge maps name -> [set:binding]
    std::map<UniformName, UniformSetPair>& baseMap = base.uniformsMap;
    const std::map<UniformName, UniformSetPair>& mergeMap = r.uniformsMap;

    for (const auto& [name, setBindingPair] : mergeMap)
    {
      const auto it = baseMap.find(name);
      if (it == baseMap.end())
      {
        baseMap[name] = setBindingPair;
        continue;
      }
      else
      {
        if (it->second != setBindingPair)
          throw std::runtime_error("Pipeline Uniforms::operator+: mapping name->[set:binding] are not equal.");
      }
    }

    return base;
  }

  PipelineUniforms SpirvParser::ParseShader(const std::vector<uint32_t>& byteCode) const
  {
    PipelineUniforms uniforms;

    spirv_cross::CompilerGLSL glsl{ byteCode };

    const spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    const spirv_cross::SmallVector<spirv_cross::EntryPoint> entryPoints = glsl.get_entry_points_and_stages();
    if (entryPoints.size() != 1)
      std::printf("Warning: Shader::ParseShader: shader has more than one entry point, using the first one.\n");

    const spirv_cross::EntryPoint entry = glsl.get_entry_points_and_stages()[0];

    const ShaderStages stage = GetShaderStage(entry);

    for (const auto& ubo : resources.uniform_buffers)
    {
      spirv_cross::SPIRType type = glsl.get_type(ubo.type_id);

      UniformBindingDescription description;
      description.size = glsl.get_declared_struct_size(type);
      description.type = UniformType::UniformBuffer;
      description.stages = stage;

      const unsigned int set = glsl.get_decoration(ubo.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int binding = glsl.get_decoration(ubo.id, spv::Decoration::DecorationBinding);

      uniforms.AddUniform(set, binding, ubo.name, description);
    }

    for (const auto& sampler : resources.sampled_images)
    {
      spirv_cross::SPIRType type = glsl.get_type(sampler.type_id);

      UniformBindingDescription description;
      description.size = 0;
      description.stages = stage;
      description.type = GetUniformTypeFromSpirvDim(type.image.dim);

      const unsigned int set = glsl.get_decoration(sampler.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int binding = glsl.get_decoration(sampler.id, spv::Decoration::DecorationBinding);

      uniforms.AddUniform(set, binding, sampler.name, description);
    }

    for (const auto& subpassInput : resources.subpass_inputs)
    {
      spirv_cross::SPIRType type = glsl.get_type(subpassInput.type_id);

      UniformBindingDescription description;
      description.size = 0;
      description.stages = stage;
      description.type = UniformType::SubpassInput;

      const unsigned int set = glsl.get_decoration(subpassInput.id, spv::Decoration::DecorationDescriptorSet);
      const unsigned int binding = glsl.get_decoration(subpassInput.id, spv::Decoration::DecorationBinding);

      uniforms.AddUniform(set, binding, subpassInput.name, description);
    }

    return uniforms;
  }
}