#include "events_queue.hpp"

namespace ecs
{
  Event* EventsQueue::popEvent()
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
}
