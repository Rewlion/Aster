#include <engine/ecs/macros.h>
#include <engine/types.h>
#include <engine/input/input.h>
#include <engine/time.h>
#include <engine/math.h>

#include <iostream>

#include <EASTL/functional.h>
#include <glm/gtx/transform.hpp>
#include "glm/gtc/quaternion.hpp"

static const bool has_input(const char* action)
{
  const int id = Engine::Input::Manager::getButtonActionId(action);
  return Engine::Input::Manager::getButtonActionState(id);
}

ECS_SYSTEM()
static void camera_movement(
  float3& pos,
  const float2& camera_rotations,
  const float3& forward
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

  const float3 up = {0.0, 1.0, 0.0};
  const float3 right = math::get_right(math::radians(camera_rotations.x));

  const float dt = Engine::Time::get_dt();
  const float v = 30.0f;

  pos += (dr.x * right + dr.y * up +  dr.z * forward) * dt * v;
}

ECS_SYSTEM()
static void camera_rotation(
  float2& camera_rotations,
  float3& forward
)
{
  const int id = Engine::Input::Manager::getAnalogActionId("CameraRotation");
  const float2 delta = Engine::Input::Manager::getAnalogActionData(id) * int2{1,-1};
  camera_rotations += delta * 0.25f;

  camera_rotations.y = glm::clamp(camera_rotations.y, -90.0f, 90.0f);

  if (camera_rotations.x > 360.0 || camera_rotations.x < -360.0)
    camera_rotations.x = 0;

  const glm::quat rotation = glm::angleAxis(math::radians(camera_rotations.x), float3{0, 1, 0}) *
                             glm::angleAxis(math::radians(camera_rotations.y), float3{-1, 0, 0});
  const mat4 tm = glm::mat4_cast(rotation);
  forward = tm * float4(0.0, 0.0, 1.0, 0.0f);
}