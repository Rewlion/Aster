#pragma once

namespace ecs
{
  template<BasedFromEcsEvent T>
  void EventsQueue::pushEvent(T&& event)
  {
    const size_t storageBegin = m_NextEventOffset;
    size_t storageEnd = storageBegin + event.eventSize;
    Sys::align(storageEnd);

    ASSERT(storageEnd < EVENTS_QUEUE_BUFFER_SIZE);

    uint8_t* rawStorage = &m_Buffer.get()[storageBegin];
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
}
