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
  class FrameGarbageCollector;

  class CompileContext
  {
    public:
      inline void init(Device* device, FrameGarbageCollector* frameGc)
      {
        m_Device = device;
        m_FrameGc = frameGc;

        m_RenderPassStorage.init(m_Device);
        m_PipelinesStorage.init(m_Device);

        for (size_t i = 0; i < std::size(m_FrameOwnedResources); ++i)
        {
          m_FrameOwnedResources[i].m_DescriptorSetsManager.init(m_Device);
        }

       acquireBackbuffer();
      }

      void compileCommand(const BeginRenderPassCmd& cmd);
      void compileCommand(const BindGraphicsShadersCmd& cmd);
      void compileCommand(const DrawCmd& cmd);
      void compileCommand(const PresentSurfaceImageCmd& cmd);
      void compileCommand(const PushConstantsCmd& cmd);
      void compileCommand(const BindVertexBufferCmd& cmd);
      void compileCommand(const BindIndexBufferCmd& cmd);
      void compileCommand(const DrawIndexedCmd& cmd);
      void compileCommand(const BindConstantBufferCmd& cmd);
      void compileCommand(const BindTextureCmd& cmd);
      void compileCommand(const BindSamplerCmd& cmd);
      void compileCommand(const ClearCmd& cmd);
      void compileCommand(const SetBlendStateCmd& cmd);
      void compileCommand(const SetDepthStencilStateCmd& cmd);

      vk::RenderPass getRenderPass(const RenderTargets& renderTargets, const TextureHandler depthStencil, const ClearState clearing);
      vk::Framebuffer getFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil);
      void updateDescriptorSets();
      void endRenderPass(const char* why);
      void insureActiveCmd();
      vk::Pipeline getPipeline(const GraphicsPipelineDescription& desc);
      bool getPipelineLayout(const ShaderStagesNames& stageNames, PipelineLayout const *& layout);

    private:
      vk::UniqueFramebuffer createFramebuffer(const vk::Extent2D& renderArea, const RenderTargets& renderTargets, const TextureHandler depthStencil);
      inline FrameOwnedResources& getCurrentFrameOwnedResources()
      {
        return m_FrameOwnedResources[m_CurrentFrame];
      }

      vk::Extent2D getMinRenderSize(const RenderTargets& renderTargets, const TextureHandler depthStencil);

      void prepareBackbufferForPresent();

      void queueGraphicsCmd();
      void submitGraphicsCmds();
      void nextFrame();
      void flushGraphicsState();

      void imageBarrier(const TextureHandler handler, const vk::ImageLayout newLayout,
                        const vk::PipelineStageFlagBits srcStage, const vk::PipelineStageFlagBits dstStage);

      void acquireBackbuffer();

    private:
      Device* m_Device = nullptr;
      FrameGarbageCollector* m_FrameGc = nullptr;
      RenderPassStorage m_RenderPassStorage;
      PipelinesStorage m_PipelinesStorage;

      FrameOwnedResources m_FrameOwnedResources[SWAPCHAIN_IMAGES_COUNT];
      size_t m_CurrentFrame = 0;

      BackendState m_State;

      eastl::vector<vk::CommandBuffer> m_QueuedGraphicsCommands;
      eastl::vector<vk::Fence> m_RenderJobWaitFences[SWAPCHAIN_IMAGES_COUNT];
      vk::Semaphore m_BackbufferReadySemaphore;
  };
}
