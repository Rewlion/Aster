#include <engine/ecs/macros.h>
#include <engine/gui/gui.h>
#include <engine/input/input.h>
#include <engine/types.h>

// ECS_SYSTEM()
// static void camera_rotation(
//   const float2& camera_rotations
// )
// {
//   const float2 pos = Engine::Input::manager.getAnalogStatus(str_hash("MouseMove"));
//   Engine::gui::manager.setMouseCursorPos(pos);
// }

// ECS_EVENT_SYSTEM()
// static void input_handler(const ButtonActionInputEvent& evt)
// {
//   if (evt.action == str_hash("MouseClick"))
//     Engine::gui::manager.setMouseClickState(evt.status == Engine::Input::ButtonStatus::Press);
// }

ECS_SYSTEM()
static void system_test_multiple_templates(const float& test_float, const string& test_str, const float2 test_float2, const float3 test_float3)
{
  volatile const float fl = test_float;
  volatile const string str = test_str;
  volatile const float2 fl2 = test_float2;
  volatile const float3 fl3 = test_float3;
}

ECS_SYSTEM()
static void system_test_default_str(const string& default_str)
{
  volatile const string str = default_str;
}

ECS_SYSTEM()
static void system_test_test_float2(const float2 test_float2)
{
  volatile const float2 fl2 = test_float2;
}