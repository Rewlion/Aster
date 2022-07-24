#pragma once

#include <shaders_compiler/spirv.h>

#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  struct PipelineLayout
  {
    vk::UniquePipelineLayout                         pipelineLayout;
    eastl::vector<vk::UniqueDescriptorSetLayout>     descriptorSetLayouts;
    eastl::vector<spirv::v2::DescriptorSet>          dsets;
    eastl::vector<vk::PipelineShaderStageCreateInfo> stagesCi;
  };
}
