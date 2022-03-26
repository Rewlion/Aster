// this file is generated by ecs codegen
// do not edit
#include <engine/ecs/registry.h>
#include <engine/ecs/components_accessor.h>
#include <EASTL/functional.h>

#include "test_query.ecs.cpp" 

using namespace Engine::ECS;

  const static query_id query_queryId = Registry::registerDirectQuery(
    DirectQueryDescription{
      .components = {
         DESCRIBE_QUERY_COMPONENT("test_float2", float2)
      }
    }
  );
  

  static void query (eastl::function<void( const float2& test_float2)> cb)
  {
    Engine::ECS::manager.query(query_queryId, [&](ComponentsAccessor& accessor)
    {
       const float2& test_float2 = accessor.get<float2>(str_hash("test_float2")) ;
      cb(test_float2);
    });
  }
  

static void event_system_internal(Event* event, ComponentsAccessor& accessor)
{
  TestEvent* casted_event = reinterpret_cast<TestEvent*>(event);
   const string& test_str = accessor.get<string>(str_hash("test_str")) ;
  event_system(*casted_event , test_str);
}
  

static const bool event_system_desc = Registry::registerCppQuery(
  QueryDescription{
    .components = {
       DESCRIBE_QUERY_COMPONENT("test_str", string)
    },
    .eventCb = event_system_internal,
    .event = str_hash("TestEvent")
  }
);
  

static void system_test_internal(ComponentsAccessor& accessor)
{
   const float& test_float = accessor.get<float>(str_hash("test_float")) ;
  system_test(test_float);
}
  

static const bool system_test_desc = Registry::registerCppQuery(
  QueryDescription{
    .cb = system_test_internal,
    .components = {
       DESCRIBE_QUERY_COMPONENT("test_float", float)
    }
  }
);
  

static void system_with_query_internal(ComponentsAccessor& accessor)
{
   const float& test_float = accessor.get<float>(str_hash("test_float")) ;
  system_with_query(test_float);
}
  

static const bool system_with_query_desc = Registry::registerCppQuery(
  QueryDescription{
    .cb = system_with_query_internal,
    .components = {
       DESCRIBE_QUERY_COMPONENT("test_float", float)
    }
  }
);
  

static void input_handler_internal(Event* event, ComponentsAccessor& accessor)
{
  ButtonActionInputEvent* casted_event = reinterpret_cast<ButtonActionInputEvent*>(event);
  
  input_handler(*casted_event );
}
  

static const bool input_handler_desc = Registry::registerCppQuery(
  QueryDescription{
    .components = {
      
    },
    .eventCb = input_handler_internal,
    .event = str_hash("ButtonActionInputEvent")
  }
);
  