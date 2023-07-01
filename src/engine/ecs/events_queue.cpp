#include "events_queue.hpp"

namespace ecs
{
  EventsQueue::EventsQueue(EventsQueue&& rvl)
  {
    std::swap(m_NextEventOffset, rvl.m_NextEventOffset);
    std::swap(m_LastEventStart, rvl.m_LastEventStart);
    std::swap(m_EventsCount, rvl.m_EventsCount);
    std::swap(m_Buffer, rvl.m_Buffer);
  }

  Event* EventsQueue::popEvent()
  {
    Event* e = nullptr;

    if (m_EventsCount > 0)
    {
      e = reinterpret_cast<Event*>(m_Buffer.get() + m_LastEventStart);

      m_NextEventOffset = m_LastEventStart;
      m_LastEventStart = e->previousAllocationOffset;

      --m_EventsCount;
    }

    return e;
  }
}
