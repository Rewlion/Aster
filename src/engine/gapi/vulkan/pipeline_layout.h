#pragma once

#include "vulkan.h"

#include <shaders_compiler/spirv.h>

namespace gapi::vulkan
{
  struct PipelineLayout
  {
    vk::UniquePipelineLayout                         pipelineLayout;
    eastl::vector<vk::UniqueDescriptorSetLayout>     descriptorSetLayouts;
    eastl::vector<spirv::v2::DescriptorSet>          dsets;
  };
}
