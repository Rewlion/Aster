#pragma once

#include <engine/gapi/resources.h>

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

      vk::RenderPass getRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing);

    private:
      vk::UniqueRenderPass createRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing);

    private:
      Device* m_Device = nullptr;

      eastl::hash_map<size_t, vk::UniqueRenderPass> m_RenderPasses;
  };
}
