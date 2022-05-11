#include "../events.h"

#include <engine/input/events.h>
#include <engine/ecs/registry.h>
#include <engine/ecs/macros.h>
#include <engine/log.h>
#include <engine/input/input.h>

#include <EASTL/functional.h>

ECS_QUERY()
static void query(eastl::function<void(const float2& test_float2)> cb);

ECS_EVENT_SYSTEM()
static void event_system(const TestEvent& event, const string& test_str)
{
  
}

ECS_SYSTEM()
static void system_test(const float& test_float)
{
  const auto status = Engine::Input::manager.getButtonStatus(str_hash("MoveLeft"));
  if (status == Engine::Input::ButtonStatus::Press)
  {
    
  }
}

ECS_SYSTEM()
static void system_with_query(const float& test_float)
{
  query([](const float2& test_float2) {
    volatile bool b = false;
  });
}

ECS_EVENT_SYSTEM()
static void input_handler(const ButtonActionInputEvent& evt)
{
}
  