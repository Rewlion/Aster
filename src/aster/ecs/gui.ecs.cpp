#include <engine/ecs/macros.h>
#include <engine/gui/gui.h>
#include <engine/input/input.h>
#include <engine/types.h>

ECS_SYSTEM()
static void camera_rotation(
  const float2& camera_rotations
)
{
  const float2 pos = Engine::Input::manager.getAnalogStatus(str_hash("MouseMove"));
  Engine::gui::manager.setMouseCursorPos(pos);
}