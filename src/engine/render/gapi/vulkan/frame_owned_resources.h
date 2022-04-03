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
      m_DescriptorSetsManager.reset();
    }

    DescriptorsSetManager m_DescriptorSetsManager;
  };
}
