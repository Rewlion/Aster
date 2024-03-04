#pragma once

#include "vulkan.h"

#include <engine/utils/bit_capacity.hpp>

#include <shaders_compiler/spirv.h>

namespace gapi::vulkan
{
  struct PipelineLayout
  {
    vk::UniquePipelineLayout                         pipelineLayout;
    eastl::vector<vk::UniqueDescriptorSetLayout>     descriptorSetLayouts;
    eastl::vector<spirv::v2::DescriptorSet>          dsets;
    Utils::BitCapacity<gapi::MAX_DESCRIPTOR_SET>     variableSizeDsets;
  };
}
