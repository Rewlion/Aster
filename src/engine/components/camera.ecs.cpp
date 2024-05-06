#include "camera.h"

#include <engine/data/ed.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>


CameraComponent::CameraComponent(const ed::Scope* init)
  : TransformComponent(init)
{
  m_LocalScale = float3{1,1,1};

  m_IsActive = init->getVariableOr("active", false);
  m_FOV = init->getVariableOr("fov", 60.0f);
  m_ZNear = init->getVariableOr("near", 0.1f);
  m_ZFar = init->getVariableOr("far", 3000.0f);
}

CameraComponent::CameraComponent(const bool is_active, const float fov,
                                 const float znear, const float zfar,
                                 const float3& pos, const float3 rot)
  : TransformComponent(pos, rot, float3{1,1,1})
  , m_IsActive(is_active)
  , m_FOV(fov)
  , m_ZNear(znear)
  , m_ZFar(zfar)
{
}

ECS_EVENT_SYSTEM()
static
void default_init_camera_component(const ecs::OnEntityCreated& evt,
                                   const TransformComponent& root_tm,
                                   CameraComponent& camera)
{
  if (!camera.hasAttachment())
    camera.attachToSelfComponent(&root_tm);
}

DECLARE_TRIVIAL_ECS_COMPONENT(CameraComponent);