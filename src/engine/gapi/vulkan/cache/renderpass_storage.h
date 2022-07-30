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

      struct CreateInfo
      {
        Utils::FixedStack<vk::AttachmentDescription, MAX_RENDER_TARGETS + 1> attachments;
        Utils::FixedStack<vk::AttachmentReference, MAX_RENDER_TARGETS + 1> attachmentsRef;
        vk::SubpassDescription subpassDesc;
        vk::RenderPassCreateInfo rpCi;
      };
      CreateInfo getCreateInfo(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing) const;
      size_t hashRenderPass(const RenderTargets& renderTargets, const RenderPassAttachment& depthStencil, const ClearState clearing) const;

    private:
      Device* m_Device = nullptr;

      eastl::hash_map<size_t, vk::UniqueRenderPass> m_RenderPasses;
  };
}
