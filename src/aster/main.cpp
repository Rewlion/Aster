#include "events.h"

#include <engine/ecs/ecs.h>
#include <engine/engine.h>

int main()
{
  Engine::init();
  Engine::start_tick();
  Engine::shutdown();
}
