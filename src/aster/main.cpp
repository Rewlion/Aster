#include <iostream>

#include <engine/engine.h>

#include <engine/ecs/registry.h>
#include <engine/ecs/components.h>
#include <engine/ecs/archetype.h>
#include <engine/ecs/components_accessor.h>

struct CmdEvent: public Event
{
  String name;

  EVENT_CONSTRUCTOR(CmdEvent)
  {
  }
};

void _event_query(CmdEvent* event, String& test_str)
{
  volatile bool f = false;
}

void event_query(Event* event, ComponentsAccessor& comps)
{
  String& str = comps.Get<String>(str_hash("test_str"));
  _event_query((CmdEvent*)(event), str);
}

int main()
{
  ::load_app_settings("game_data/settings.blk");
  ::init_log();
  ::init_window();

  ecs.register_event(str_hash("CmdEvent"));

  QueryDescription query;
  query.eventCb = event_query;
  query.event = str_hash("CmdEvent");

  bool b = ecs.register_cpp_query(std::move(query));


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
    CmdEvent event; event.name = "vv";
    ecs.broadcast_event(event);
    event.name = "mmmm!!";
    ecs.broadcast_event(event);

    ::poll_input_events();

    ecs.tick();
  }
}
