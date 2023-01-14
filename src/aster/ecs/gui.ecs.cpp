#include <engine/ecs/macros.h>
#include <engine/input/input.h>
#include <engine/types.h>

#include <engine/log.h>

ECS_SYSTEM()
static void camera_rotation(
  const float2& camera_rotations
)
{
  const float2 pos = Engine::Input::manager.getAnalogStatus(str_hash("MouseMove"));
  logerror("pos: {} {}", pos.x, pos.y);
}