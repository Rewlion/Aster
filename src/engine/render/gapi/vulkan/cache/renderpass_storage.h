#pragma once

#include <engine/render/gapi/resources.h>
#include <engine/render/gapi/commands.h>

#include <vulkan/vulkan.hpp>
#include <EASTL/hash_map.h>

namespace gapi::vulkan
{
  class Device;

  class RenderPassStorage
  {
    public:
      inline void Init(Device* device)
      {
        m_Device = device;
      }

      vk::RenderPass GetRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing);

    private:
      vk::UniqueRenderPass CreateRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing);

    private:
      Device* m_Device = nullptr;

      eastl::hash_map<size_t, vk::UniqueRenderPass> m_RenderPasses;
  };
}
