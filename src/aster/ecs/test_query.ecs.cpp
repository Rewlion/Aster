#include "../events.h"

#include <engine/ecs/registry.h>
#include <engine/ecs/macros.h>

#include <EASTL/functional.h>

ECS_QUERY()
static void query(eastl::function<void(const Float2& test_float2)> cb);

ECS_EVENT_SYSTEM()
static void event_system(const TestEvent& event, const String& test_str)
{
  
}

ECS_SYSTEM()
static void system_test(const float& test_float)
{
  
}

ECS_SYSTEM()
static void system_with_query(const float& test_float)
{
  query([](const Float2& test_float2) {
    volatile bool b = false;
  });
}
