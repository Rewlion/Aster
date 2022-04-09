#pragma once

#include "vulkan.h"

#include <EASTL/vector.h>
#include "vulkan/vulkan.hpp"

namespace gapi::vulkan
{
  class FrameGarbageCollector
  {
    public:

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

      inline void addFramebuffer(vk::UniqueFramebuffer&& fb)
      {
        frameResources[frameId].framebuffers.push_back(std::move(fb));
      }

      inline void nextFrame()
      {
        frameId = (frameId + 1) % SWAPCHAIN_IMAGES_COUNT;

        frameResources[frameId].buffers.clear();
        frameResources[frameId].memories.clear();
        frameResources[frameId].cmdBuffers.clear();
        frameResources[frameId].semaphores.clear();
        frameResources[frameId].fences.clear();
        frameResources[frameId].framebuffers.clear();

      }

    private:
      struct Resources
      {
        eastl::vector<vk::UniqueBuffer> buffers;
        eastl::vector<vk::UniqueDeviceMemory> memories;
        eastl::vector<vk::UniqueCommandBuffer> cmdBuffers;
        eastl::vector<vk::UniqueSemaphore> semaphores;
        eastl::vector<vk::UniqueFence> fences;
        eastl::vector<vk::UniqueFramebuffer> framebuffers;
      };

      size_t frameId = 0;
      Resources frameResources[SWAPCHAIN_IMAGES_COUNT];
  };
}
