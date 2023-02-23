#include <engine/ecs/events.h>

struct TestEvent: public ecs::Event
{
  string testString;

  EVENT_CONSTRUCTOR(TestEvent)
  {

  }
};
