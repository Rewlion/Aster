#include <engine/components/camera.h>
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
  CameraComponent& camera
)
{
  if (!camera.isActive())
    return;

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
  const float4x4 tm = camera.getLocalTransform();
  const float3 right = tm[0];
  const float3 forward = tm[2];

  const float dt = Engine::Time::get_dt();
  const float v = 50.0f;

  float3 pos = camera.getLocalPosition();
  pos += (dr.x * right + dr.y * up +  dr.z * forward) * dt * v;

  camera.setLocalPosition(pos);
}

ECS_SYSTEM()
static void camera_rotation(
  CameraComponent& camera
)
{
  if (camera.isActive())
  {
    const int id = Engine::Input::Manager::getAnalogActionId("CameraRotation");
    const float2 delta = (float2)Engine::Input::Manager::getAnalogActionData(id) * float2{-1.0f,-1.0f} * 0.25f;
    const float3 rot = camera.getLocalRotation();

    const float newPitch = glm::clamp(rot.y + delta.y, -89.0f, 89.0f);
    float newYaw = rot.z + delta.x;
    newYaw = newYaw < -360.0 || newYaw > 360.0 ? 0.0 : newYaw;

    camera.setLocalRotation({
      0.0f,
      newPitch,
      newYaw
    });
  }
}