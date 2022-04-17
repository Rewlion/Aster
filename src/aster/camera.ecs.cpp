#include <engine/ecs/macros.h>
#include <engine/types.h>
#include <engine/input/input.h>
#include <engine/time.h>

#include <EASTL/functional.h>

static const bool has_input(const char* action)
{
  const auto status = Engine::Input::manager.getKeyboardButtonStatus(str_hash(action));
  return status == Engine::Input::ButtonStatus::Press;
}

ECS_SYSTEM()
static void camera_movement(
  float3& pos,
  const float3& forward,
  const float3& right
)
{
  float3 dr  = {0,0,0};
  if (has_input("MoveLeft"))
    dr.x += -1.0;
  if (has_input("MoveRight"))
    dr.x += 1.0;

  if (has_input("MoveUp"))
    dr.y += +1.0;
  if (has_input("MoveDown"))
    dr.y -= 1.0;

  if (has_input("MoveForward"))
    dr.z += +1.0;
  if (has_input("MoveBackward"))
    dr.z -= 1.0;

  const float3 up = {0.0, -1.0, 0.0};

  const float dt = Engine::Time::get_dt();
  const float v = 1.0f;

  pos += (dr.x * right + dr.y * up +  dr.z * forward) * dt;
}