#include <engine/ecs/events.h>

struct TestEvent: public Engine::ECS::Event
{
  string testString;

  EVENT_CONSTRUCTOR(TestEvent)
  {

  }
};
