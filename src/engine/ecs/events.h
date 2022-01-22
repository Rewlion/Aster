#pragma once

#include "types.h"

#include <engine/algorithm/hash.h>
#include <engine/assert.h>
#include <engine/platform/memory.h>

#include <EASTL/fixed_vector.h>

#include <type_traits>

#ifdef __clang__
  #define ECS_EVENT_BASE() [[clang::annotate("ecs_event_base_struct")]]
#else
  #define ECS_EVENT_BASE()
#endif

#define EVENT_STATIC_DESTR_NAME(eventStruct) eventStruct ## _destructor

#define BASE_EVENT_CONSTRUCTOR(eventStruct) Event( str_hash(#eventStruct), sizeof(eventStruct),  EVENT_STATIC_DESTR_NAME(eventStruct))

#define EVENT_CONSTRUCTOR(eventStruct)\
  static void EVENT_STATIC_DESTR_NAME(eventStruct)(Event* event)\
  { \
    eventStruct* e = reinterpret_cast<eventStruct*>(event);\
    e->~eventStruct();\
  }\
  eventStruct()\
    : BASE_EVENT_CONSTRUCTOR(eventStruct)

constexpr size_t EVENTS_QUEUE_BUFFER_SIZE = 1024;

namespace Engine::ECS
{
  struct ECS_EVENT_BASE() Event;

  using EventDestructorPtr = void(Event*);

  struct ECS_EVENT_BASE() Event
  {
    friend class EventsQueue;
    friend class Registry;

    Event(event_hash_name hashName, size_t size, void (*destr)(Event*))
      : eventNameHash(hashName)
      , eventSize(size)
      , _destr(destr)
    {
    }

    private:
      event_hash_name eventNameHash = 0;
      size_t eventSize = 0;
      size_t allocationSize = 0;
      size_t previousAllocationOffset = 0;
      void (*_destr)(Event* );
  };

  class EventsQueue
  {
    public:
      template<class T>
      inline void push_event(T&& event)
      {
        const size_t storageBegin = m_NextEventOffset;
        size_t storageEnd = storageBegin + event.eventSize;
        Sys::Align(storageEnd);

        ASSERT(storageEnd < EVENTS_QUEUE_BUFFER_SIZE);

        uint8_t* rawStorage = &m_Buffer[storageBegin];
        typedef std::remove_reference<T>::type unrefT;
        unrefT* storage = new(rawStorage) unrefT;

        *storage = eastl::forward<T>(event);

        auto* allocatedEvent = reinterpret_cast<Event*>(storage);
        allocatedEvent->allocationSize =  storageEnd - storageBegin;
        allocatedEvent->previousAllocationOffset = m_LastEventStart;

        m_LastEventStart = storageBegin;
        m_NextEventOffset = storageEnd;
        ++m_EventsCount;
      }

      inline Event* pop_event()
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
