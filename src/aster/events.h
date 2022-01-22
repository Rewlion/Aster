#include <engine/ecs/events.h>

struct TestEvent: public Event
{
  string testString;

  EVENT_CONSTRUCTOR(TestEvent)
  {

  }
};
