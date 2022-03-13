#pragma once

#include "frame_owned_resources.h"
#include "resources.h"

#include <engine/render/gapi/vulkan/cache/renderpass_storage.h>
#include <engine/render/gapi/vulkan/cache/pipelines_storage.h>


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

        for (size_t i = 0; i < std::size(m_FrameOwnedResources); ++i)
        {
          m_FrameOwnedResources->m_DescriptorSetsManager.Init(m_Device);
        }
      }

      void compileCommand(const BeginRenderPassCmd& cmd);
      void compileCommand(const EndRenderPassCmd& cmd);
      void compileCommand(const BindGraphicsPipelineCmd& cmd);
      void compileCommand(const DrawCmd& cmd);
      void compileCommand(const PresentSurfaceImageCmd& cmd);
      void compileCommand(const PushConstantsCmd& cmd);
      void compileCommand(const BindVertexBufferCmd& cmd);
      void compileCommand(const BindIndexBufferCmd& cmd);
      void compileCommand(const DrawIndexedCmd& cmd);
      void compileCommand(const BindTextureCmd& cmd);
      void compileCommand(const BindSamplerCmd& cmd);

    private:
      vk::UniqueFramebuffer createFramebuffer(const BeginRenderPassCmd& cmd, const vk::RenderPass& rp);
      inline FrameOwnedResources& GetCurrentFrameOwnedResources()
      {
        return m_FrameOwnedResources[m_CurrentFrame];
      }

      void UpdateViewport(const BeginRenderPassCmd& cmd);

      void NextFrame();

    private:
      Device* m_Device = nullptr;
      RenderPassStorage m_RenderPassStorage;
      PipelinesStorage m_PipelinesStorage;

      vk::CommandBuffer m_CurrentCmdBuf;
      vk::UniqueCommandPool m_CurrentFramebuffer;

      FrameOwnedResources m_FrameOwnedResources[SWAPCHAIN_IMAGES_COUNT];
      size_t m_CurrentFrame = 0;

      vk::RenderPass m_CurrentRenderPass;
      size_t m_CurrentSubpass = 0;

      vk::Extent2D m_CurrentViewportDim = {0,0};

      ShaderStagesNames m_CurrentPipelineStages;
  };
}
