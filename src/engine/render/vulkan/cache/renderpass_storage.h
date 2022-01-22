#pragma once

#include <engine/render/resources.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  class RenderPassStorage
  {
    public:
      inline void init(Device* device)
      {
        m_Device = device;
      }

      vk::RenderPass getRenderPass(const BeginRenderPassCmd& cmd);

    private:
      vk::UniqueRenderPass createRenderPass(const BeginRenderPassCmd& cmd);

    private:
      Device* m_Device = nullptr;

      eastl::hash_map<size_t, vk::UniqueRenderPass> m_RenderPasses;
  };
}
