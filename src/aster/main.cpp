#include <iostream>

#include <engine/engine.h>

#include <engine/ecs/registry.h>
#include <engine/ecs/components.h>
#include <engine/ecs/archetype.h>
#include <engine/ecs/components_accessor.h>

int main()
{
  ::load_app_settings("game_data/settings.blk");
  ::init_log();
  ::init_window();

  ::init_ecs_from_settings();

  ecs.create_entity(str_hash("template1"),
  [](const EntityId& eid, EntityInitializer& init){
    init.init_component<Float2>(str_hash("test_float2"), Float2{23.0f, 1.0f});
    init.init_component<float>(str_hash("test_float"), 274.0);
    init.init_component<String>(str_hash("test_str"), "LOL");
  });

  ecs.create_entity(str_hash("template2"),
    [](const EntityId& eid, EntityInitializer& init) {
  });

  for(;;)
  {
    ::poll_input_events();

    ecs.tick();
  }
}
