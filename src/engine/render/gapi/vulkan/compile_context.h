#pragma once

#include "frame_owned_resources.h"
#include "resources.h"
#include "graphics_state.h"

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
      void compileCommand(const BindGraphicsPipelineCmd& cmd);
      void compileCommand(const DrawCmd& cmd);
      void compileCommand(const PresentSurfaceImageCmd& cmd);
      void compileCommand(const PushConstantsCmd& cmd);
      void compileCommand(const BindVertexBufferCmd& cmd);
      void compileCommand(const BindIndexBufferCmd& cmd);
      void compileCommand(const DrawIndexedCmd& cmd);
      void compileCommand(const BindTextureCmd& cmd);
      void compileCommand(const BindSamplerCmd& cmd);
      void compileCommand(const ClearCmd& cmd);

      vk::RenderPass GetRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing);
      vk::Framebuffer GetFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil);
      void UpdateDescriptorSets();
      void EndRenderPass(const char* why);
      void InsureActiveCmd();
      vk::Pipeline GetPipeline(const GraphicsPipelineDescription& desc);
      bool GetPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout);
      void SubmitGraphicsCmd();

    private:
      vk::UniqueFramebuffer CreateFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil);
      inline FrameOwnedResources& GetCurrentFrameOwnedResources()
      {
        return m_FrameOwnedResources[m_CurrentFrame];
      }

      vk::Extent2D GetMinRenderSize(const RenderTargets& renderTargets, const TextureHandler depthStencil);

      void NextFrame();
      void FlushGraphicsState();

    private:
      Device* m_Device = nullptr;
      RenderPassStorage m_RenderPassStorage;
      PipelinesStorage m_PipelinesStorage;

      FrameOwnedResources m_FrameOwnedResources[SWAPCHAIN_IMAGES_COUNT];
      size_t m_CurrentFrame = 0;

      bool m_HasActiveRp = false;
      vk::Framebuffer m_CurrentFramebuffer;
      Utils::FixedStack<vk::ClearValue, MAX_RENDER_TARGETS+1> m_CurrentClearValues;

      BackendState m_State;
  };
}
