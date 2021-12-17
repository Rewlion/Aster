#include <engine/ecs/events.h>

struct TestEvent: public Event
{
  String testString;

  EVENT_CONSTRUCTOR(TestEvent)
  {

  }
};
