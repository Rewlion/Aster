#include <engine/ecs/registry.h>
#include <EASTL/functional.h>

[[clang::annotate("ecs_query")]]
static void query(eastl::function<void(const Float2& test_float2)> cb);


[[clang::annotate("ecs_system")]]
static void system_test(const float& test_float)
{
  
}

[[clang::annotate("ecs_system")]]
static void system_with_query(const float& test_float)
{
  query([](const Float2& test_float2) {
    volatile bool b = false;
  });
}
