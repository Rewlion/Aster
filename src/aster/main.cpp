#include "events.h"

#include <engine/ecs/ecs.h>
#include <engine/engine.h>

void register_aster_events()
{
  ecs::get_registry().registerEvent<TestEvent>();
}

int main()
{
  Engine::init();
  register_aster_events();

  Engine::start_tick();
}
