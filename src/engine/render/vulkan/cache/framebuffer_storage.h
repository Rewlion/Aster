#pragma once

#include <engine/render/resources.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  class FramebufferStorage
  {
    public:
      vk::Framebuffer getFramebuffer()

    private:
      Device* m_Device;
      eastl::hash_map<size_t, vk::UniqueFramebuffer> m_Framebuffers;
  };
}
