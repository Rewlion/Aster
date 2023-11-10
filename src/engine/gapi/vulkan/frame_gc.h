#pragma once

#include "vulkan.h"
#include "descriptors_set_manager.h"
#include "result.h"

#include <EASTL/vector.h>

#include <engine/debug_marks.h>
#include <engine/log.h>

namespace gapi::vulkan
{
  constexpr size_t GC_FRAMES = SWAPCHAIN_IMAGES_COUNT+1;

  class FrameGarbageCollector
  {
    public:
      inline void init(Device* d)
      {
        device = d;
      }


      inline void addBuffer(Buffer&& b)
      {
        addBuffer(std::move(b.buffer));
        addMemory(std::move(b.memory));
      }

      inline void addBuffer(vk::UniqueBuffer&& buffer)
      {
        frameResources[frameId].buffers.push_back(std::move(buffer));
      }

      inline void addMemory(vk::UniqueDeviceMemory&& memory)
      {
        frameResources[frameId].memories.push_back(std::move(memory));
      }

      inline void addCmdBuffer(vk::UniqueCommandBuffer&& cmdBuffer)
      {
        frameResources[frameId].cmdBuffers.push_back(std::move(cmdBuffer));
      }

      inline void addSemaphores(vk::UniqueSemaphore&& semaphore)
      {
        frameResources[frameId].semaphores.push_back(std::move(semaphore));
      }

      inline void addFence(vk::UniqueFence&& fence)
      {
        frameResources[frameId].fences.push_back(std::move(fence));
      }

      inline void addWaitFence(vk::UniqueFence&& fence)
      {
        frameResources[frameId].waitFences.push_back(fence.get());
        frameResources[frameId].fences.push_back(std::move(fence));
      }

      inline void addFramebuffer(vk::UniqueFramebuffer&& fb)
      {
        frameResources[frameId].framebuffers.push_back(std::move(fb));
      }

      inline void addDsetManager(DescriptorsSetManager&& dsetManager)
      {
        frameResources[frameId].dsetManagers.push_back(std::move(dsetManager));
      }

      inline void addCmdPool(vk::UniqueCommandPool&& cmdPool)
      {
        frameResources[frameId].cmdPools.push_back(std::move(cmdPool));
      }

      inline void addEvent(vk::UniqueEvent&& event)
      {
        frameResources[frameId].events.push_back(std::move(event));
      }

      inline void nextFrame()
      {
        PROFILE_CPU_NAMED("FrameGC clearing");

        frameId = (frameId + 1) % GC_FRAMES;
        {
          auto& waitFences = frameResources[frameId].waitFences;
          if (waitFences.size() > 0)
          {
            VK_CHECK(device->getDevice().waitForFences(waitFences.size(), waitFences.data(), true, -1));
            waitFences.clear();
          }
        }

        frameResources[frameId].buffers.clear();
        frameResources[frameId].memories.clear();
        frameResources[frameId].cmdBuffers.clear();
        frameResources[frameId].semaphores.clear();
        frameResources[frameId].fences.clear();
        frameResources[frameId].framebuffers.clear();
        frameResources[frameId].dsetManagers.clear();
        frameResources[frameId].cmdPools.clear();
        frameResources[frameId].events.clear();
      }

      inline
      void clearAllFrames()
      {
        for (int i = 0; i < GC_FRAMES; ++i)
          nextFrame();
      }

    private:
      struct Resources
      {
        eastl::vector<vk::Fence> waitFences;
        eastl::vector<vk::UniqueBuffer> buffers;
        eastl::vector<vk::UniqueDeviceMemory> memories;
        eastl::vector<vk::UniqueCommandBuffer> cmdBuffers;
        eastl::vector<vk::UniqueSemaphore> semaphores;
        eastl::vector<vk::UniqueFence> fences;
        eastl::vector<vk::UniqueFramebuffer> framebuffers;
        eastl::vector<DescriptorsSetManager> dsetManagers;
        eastl::vector<vk::UniqueCommandPool> cmdPools;
        eastl::vector<vk::UniqueEvent> events;
      };

      Device* device = nullptr;
      size_t frameId = 0;
      Resources frameResources[GC_FRAMES];
  };
}
