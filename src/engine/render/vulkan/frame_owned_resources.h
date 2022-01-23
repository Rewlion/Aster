#pragma once

#include <EASTL/vector.h>
#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  struct FrameOwnedResources
  {
    inline void Clear()
    {
      m_Framebuffers.clear();
    }

    eastl::vector<vk::UniqueFramebuffer> m_Framebuffers;
  };
}
