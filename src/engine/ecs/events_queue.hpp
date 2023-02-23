#pragma once

#include "events.h"

#include <engine/assert.h>
#include <engine/platform/memory.h>

#include <EASTL/vector.h>

namespace ecs
{
  class EventsQueue
  {
    constexpr static size_t EVENTS_QUEUE_BUFFER_SIZE = 1024;

    public:
      template<BasedFromEcsEvent T>
      void pushEvent(T&& event);

      Event* popEvent();

    private:
      size_t m_NextEventOffset = 0;
      size_t m_LastEventStart = 0;
      size_t m_EventsCount = 0;
      uint8_t m_Buffer[EVENTS_QUEUE_BUFFER_SIZE];
  };
}

#include "events_queue.inc.hpp"
