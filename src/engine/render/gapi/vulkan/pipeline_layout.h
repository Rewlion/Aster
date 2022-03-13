#pragma once

#include <engine/shaders_compiler/spirv.h>

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  struct PipelineLayout
  {
    vk::UniquePipelineLayout      pipelineLayout;
    vk::UniqueDescriptorSetLayout descriptorSetLayouts[spirv::MAX_SETS_COUNT];
    spirv::ShaderArgument         sets[spirv::MAX_SETS_COUNT];
  };
}
