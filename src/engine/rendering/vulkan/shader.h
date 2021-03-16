#pragma once

#include "shader_parsing.h"

#include <vulkan/vulkan.hpp>

#include <string>

namespace Vulkan
{
  class Core;

  class Shader
  {
  public:
    Shader(vk::Device logicalDevice, const std::vector<uint32_t>& byteCode);

    vk::ShaderModule GetModule() const;

    inline const PipelineUniforms& GetUniformsDescriptions() const
    {
      return uniforms;
    }

  private:
    vk::UniqueShaderModule shaderModule;

    PipelineUniforms uniforms;
  };

  class ShaderProgram
  {
  public:
    ShaderProgram(Core& core, Shader&& vertex, Shader&& fragment);
    ~ShaderProgram();

    inline const Shader& GetVertexShader() const
    {
      return vertex;
    }

    inline const Shader& GetFragmentShader() const
    {
      return fragment;
    }

    inline const PipelineUniforms& GetCombinedUniformsInformation() const
    {
      return uniforms;
    }

    inline const std::vector<vk::DescriptorSetLayout> GetLayouts() const
    {
      return layouts;
    }

    inline std::string GetID() const
    {
      return id;
    }

  private:
    std::vector<vk::DescriptorSetLayout> CreateLayouts(Core& core, const PipelineUniforms& uniforms) const;

  private:
    Core& core;

    Shader vertex;
    Shader fragment;

    PipelineUniforms uniforms;
    std::vector<vk::DescriptorSetLayout> layouts;

    std::string id;
  };

}