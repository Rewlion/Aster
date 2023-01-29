#pragma once

#include "events.h"

#include <engine/assert.h>
#include <engine/platform/memory.h>

#include <EASTL/vector.h>

namespace Engine::ECS
{
  class EventsQueue
  {
    constexpr static size_t EVENTS_QUEUE_BUFFER_SIZE = 1024;

    public:
      template<BasedFromEcsEvent T>
      inline void pushEvent(T&& event)
      {
        const size_t storageBegin = m_NextEventOffset;
        size_t storageEnd = storageBegin + event.eventSize;
        Sys::align(storageEnd);

        ASSERT(storageEnd < EVENTS_QUEUE_BUFFER_SIZE);

        uint8_t* rawStorage = &m_Buffer[storageBegin];
        typedef typename std::remove_reference<T>::type unrefT;
        unrefT* storage = new(rawStorage) unrefT;

        *storage = eastl::forward<T>(event);

        auto* allocatedEvent = reinterpret_cast<Event*>(storage);
        allocatedEvent->allocationSize =  storageEnd - storageBegin;
        allocatedEvent->previousAllocationOffset = m_LastEventStart;

        m_LastEventStart = storageBegin;
        m_NextEventOffset = storageEnd;
        ++m_EventsCount;
      }

      inline Event* popEvent()
      {
        Event* e = nullptr;

        if (m_EventsCount > 0)
        {
          e = reinterpret_cast<Event*>(m_Buffer + m_LastEventStart);

          m_NextEventOffset = m_LastEventStart;
          m_LastEventStart = e->previousAllocationOffset;

          --m_EventsCount;
        }

        return e;
      }

    private:
    size_t m_NextEventOffset = 0;
    size_t m_LastEventStart = 0;
    size_t m_EventsCount = 0;
    uint8_t m_Buffer[EVENTS_QUEUE_BUFFER_SIZE];
  };
}
