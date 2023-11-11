#pragma once

#include "descriptors_set_manager.h"
#include "device.h"
#include "resources.h"
#include "result.h"

#include <engine/debug_marks.h>
#include <engine/log.h>

#include <tuple>

namespace gapi::vulkan
{
  constexpr size_t GC_FRAMES = SWAPCHAIN_IMAGES_COUNT+1;
  
  template<class ...ResourceType>
  class FrameGarbageCollectorBase
  {
    class DestroyQueue
    {
      public:
        template<class T>
        void add(T&& res)
        {
          eastl::vector<T>& pool = getPool<T>();
          pool.push_back(std::move(res));
        }

        void destroy()
        {
          std::apply([](auto& ...resource){
            (resource.clear(), ...);
          }, m_Resources);
        }
      private:
        template<class Resource>
        auto getPool() -> eastl::vector<Resource>&
        {
          return std::get<eastl::vector<Resource>>(m_Resources);
        }

        std::tuple<eastl::vector<ResourceType>...> m_Resources;
    };

    class WaitQueue
    {
      public:
        WaitQueue() = default;
        WaitQueue(Device* device)
          : m_Device(device)
        {
        }

        void add(const vk::Fence fence)
        {
          m_Fences.push_back(fence);
        }

        void wait()
        {
          if (m_Fences.size() > 0)
          {
            VK_CHECK(m_Device->getDevice().waitForFences(m_Fences.size(), m_Fences.data(), true, -1));
            m_Fences.clear();
          }
        }
      private:
        Device* m_Device = nullptr;
        eastl::vector<vk::Fence> m_Fences;
    };

  public:
    void init(Device *d)
    {
      m_FrameId = 0;
      for (size_t i = 0; i < GC_FRAMES; ++i)
        m_WaitQueue[i] = WaitQueue{d};
    }

    void addBuffer(Buffer &&b)
    {
      add(std::move(b.buffer));
      add(std::move(b.memory));
    }

    void addWaitFence(vk::UniqueFence&& fence)
    {
      m_WaitQueue[m_FrameId].add(fence.get());
      m_DestroyQueue[m_FrameId].add(std::move(fence));
    }

    template<class T>
    void add(T&& res)
    {
      m_DestroyQueue[m_FrameId].add(std::move(res));
    }

    void nextFrame()
    {
      PROFILE_CPU_NAMED("FrameGC clearing");
      m_FrameId = (m_FrameId + 1) % GC_FRAMES;
      m_WaitQueue[m_FrameId].wait();
      m_DestroyQueue[m_FrameId].destroy();
    }

    void clearAllFrames()
    {
      for (int i = 0; i < GC_FRAMES; ++i)
        nextFrame();
    }

  private:
    size_t m_FrameId = 0;
    DestroyQueue m_DestroyQueue [GC_FRAMES];
    WaitQueue m_WaitQueue [GC_FRAMES];
  };

  class FrameGarbageCollector
    : public FrameGarbageCollectorBase<
      vk::UniqueBuffer, vk::UniqueDeviceMemory, vk::UniqueCommandBuffer,
      vk::UniqueSemaphore, vk::UniqueFence, vk::UniqueFramebuffer, DescriptorsSetManager,
      vk::UniqueCommandPool, vk::UniqueEvent>
  {
  };
}

#undef GC_TYPES