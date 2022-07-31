#pragma once

#include "vulkan.h"
#include "descriptors_set_manager.h"
#include "result.h"

#include <EASTL/vector.h>

namespace gapi::vulkan
{
  class FrameGarbageCollector
  {
    public:
      inline void init(Device* d)
      {
        device = d;
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

      inline void nextFrame()
      {
        frameId = (frameId + 1) % SWAPCHAIN_IMAGES_COUNT;
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
      };

      Device* device = nullptr;
      size_t frameId = 0;
      Resources frameResources[SWAPCHAIN_IMAGES_COUNT];
  };
}
