#pragma once

#include <engine/render/vulkan/cache/renderpass_storage.h>
#include <engine/render/vulkan/cache/pipelines_storage.h>

#include "resources.h"

#include <EASTL/vector.h>
#include <vulkan/vulkan.hpp>

namespace gapi::vulkan
{
  class Device;

  class CompileContext
  {
    public:
      inline void init(Device* device)
      {
        m_Device = device;
        m_RenderPassStorage.Init(m_Device);
        m_PipelinesStorage.Init(m_Device);
      }

      void compileCommand(const BeginRenderPassCmd& cmd);
      void compileCommand(const EndRenderPassCmd& cmd);
      void compileCommand(const BindGraphicsPipelineCmd& cmd);

    private:
      vk::UniqueFramebuffer createFramebuffer(const BeginRenderPassCmd& cmd, const vk::RenderPass& rp);

    private:
      Device* m_Device = nullptr;
      RenderPassStorage m_RenderPassStorage;
      PipelinesStorage m_PipelinesStorage;

      vk::CommandBuffer m_CurrentCmdBuf;
      vk::UniqueCommandPool m_CurrentFramebuffer;

      struct FrameResources
      {
        eastl::vector<vk::UniqueFramebuffer> framebuffers;
      };
      FrameResources m_Frames[SWAPCHAIN_IMAGES_COUNT];
      size_t m_CurrentFrame = 0;

      vk::RenderPass m_CurrentRenderPass;
      size_t m_CurrentSubpass = 0;
  };
}
