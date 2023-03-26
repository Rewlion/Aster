#pragma once

#include "core_types.h"
#include "eid.h"
#include "hash.h"

#include <engine/algorithm/hash.h>

#include <type_traits>

#ifdef __clang__
  #define ECS_EVENT_BASE() [[clang::annotate("ecs_event_base_struct")]]
#else
  #define ECS_EVENT_BASE()
#endif

#define EVENT_STATIC_DESTR_NAME(eventStruct) eventStruct ## _destructor

#define BASE_EVENT_CONSTRUCTOR(eventStruct) Event(#eventStruct, compile_ecs_name_hash(#eventStruct), sizeof(eventStruct),  EVENT_STATIC_DESTR_NAME(eventStruct))

#define EVENT_CONSTRUCTOR(eventStruct)\
  static constexpr const char* eventName = #eventStruct;\
  static void EVENT_STATIC_DESTR_NAME(eventStruct)(Event* event)\
  { \
    eventStruct* e = reinterpret_cast<eventStruct*>(event);\
    e->~eventStruct();\
  }\
  eventStruct()\
    : BASE_EVENT_CONSTRUCTOR(eventStruct)

namespace ecs
{
  struct ECS_EVENT_BASE() Event;

  using EventDestructorPtr = void(Event*);

  struct ECS_EVENT_BASE() Event
  {
    friend class EventsQueue;
    friend class Registry;

    Event(const char* name, name_hash_t hashName, size_t size, void (*destr)(Event*))
      : name(name)
      , eventNameHash(hashName)
      , eventSize(size)
      , _destr(destr)
    {
    }

    private:
      const char* name = nullptr;
      name_hash_t eventNameHash{0};
      size_t eventSize = 0;
      size_t allocationSize = 0;
      size_t previousAllocationOffset = 0;
      EntityId receiver;
      void (*_destr)(Event* );
  };

  template<class T>
  concept BasedFromEcsEvent = std::is_base_of_v<Event, T>;
}
