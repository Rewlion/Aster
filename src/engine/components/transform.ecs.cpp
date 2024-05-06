#include "transform.h"

#include <engine/data/ed.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/type_meta.h>
#include <engine/math.h>
#include <engine/utils/pattern_matching.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

TransformComponent::TransformComponent(const ed::Scope* init)
{
  m_LocalPosition = init->getVariableOr("position", float3(0,0,0));
  m_LocalRotation = init->getVariableOr("rotation", float3(0,0,0));
  m_LocalScale = init->getVariableOr("scale", float3{1,1,1});
}

void TransformComponent::attachToEntity(const ecs::EntityId eid, const TransformComponent* entity_comp)
{
  m_ParentAttachment = EntityAttachment {
    .eid = eid,
    .comp = entity_comp
  };
}

void TransformComponent::attachToSelfComponent(const TransformComponent* comp)
{
  m_ParentAttachment = SelfAttachment {
    .comp = comp
  };
}

auto TransformComponent::getParentTransform() const -> const TransformComponent*
{
  return m_ParentAttachment >> match {
    [] (auto)
    {
      return (const TransformComponent*)nullptr;
    },
    [] (const EntityAttachment& att)
    {
      return ecs::get_registry().isEntityAlive(att.eid) ?
              att.comp :
              (const TransformComponent*)nullptr;
    },
    [] (const SelfAttachment& att) {
      return att.comp;
    }
  };
}

auto TransformComponent::getWorldTransformTmInternal(const TransformComponent* comp) const -> float4x4
{
  if (comp) [[likely]]
  {
    const TransformComponent* parentAttachment = comp->getParentTransform();
    const float4x4 parentTm = comp->getWorldTransformTmInternal(parentAttachment);
    const float4x4 localTm = comp->getLocalTransform();

    return localTm * parentTm;
  }

  return float4x4{1.0};
}

auto TransformComponent::getWorldTransform() const -> float4x4
{
  return getWorldTransformTmInternal(this);
}

auto TransformComponent::getLocalTransform() const -> float4x4
{
  const float4x4 trTm = glm::translate(m_LocalPosition);
  const float4x4 scaleTm = glm::scale(m_LocalScale);

  const auto [roll, pitch, yaw] = m_LocalRotation;
  const float4x4 rotTm = math::rotate(math::radians(roll), math::radians(pitch), math::radians(yaw));

  return trTm * rotTm * scaleTm;
}

auto TransformComponent::getWorldPosition() const -> float3
{
  return getWorldTransform()[3];
}

auto TransformComponent::getWorldForward() const -> float3
{
  const float4x4 tm = getWorldTransform();
  return glm::normalize(tm[2]);
}

DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(TransformComponent, "TransformComponent");