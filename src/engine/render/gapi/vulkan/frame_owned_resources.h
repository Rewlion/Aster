#pragma once

#include "descriptors_set_manager.h"

#include <EASTL/vector.h>
#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  struct FrameOwnedResources
  {
    inline void Clear()
    {
      m_Framebuffers.clear();
      m_DescriptorSetsManager.reset();
    }

    eastl::vector<vk::UniqueFramebuffer> m_Framebuffers;
    DescriptorsSetManager m_DescriptorSetsManager;
  };
}
