#include "transform.h"

#include <engine/data/ed.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/type_meta.h>
#include <engine/math.h>
#include <engine/utils/pattern_matching.hpp>
#include <engine/reflection/field.h>
#include <engine/reflection/class.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

class TransformComponentClass : public Class
{
  public:
    auto getFieldsBegin() const -> const ClassField* override
    {
      return m_Fields;
    }
    
    auto getFieldsCount() const -> size_t override
    {
      return m_FieldsCount;
    }
    
  private:
    static ClassField m_Fields[];
    static size_t m_FieldsCount;
};

ClassField TransformComponentClass::m_Fields[] = {
  ClassField{
    .offset = offsetof(TransformComponent, m_LocalPosition),
    .name = "Position",
    .type = ClassField::Type::Float3
  },
  ClassField{
    .offset = offsetof(TransformComponent, m_LocalRotation),
    .name = "Rotation",
    .type = ClassField::Type::Float3
  },
  ClassField{
    .offset = offsetof(TransformComponent, m_LocalScale),
    .name = "Scale",
    .type = ClassField::Type::Float3
  },
};

size_t TransformComponentClass::m_FieldsCount = std::size(TransformComponentClass::m_Fields);

template<>
auto get_static_class<TransformComponent>() -> const Class*
{
  static TransformComponentClass c;
  return &c;
}

auto TransformComponent::getClass() -> const Class*
{
  return getStaticClass();
}

auto TransformComponent::getStaticClass() -> const Class*
{
  return get_static_class<TransformComponent>();
}

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

    return parentTm * localTm;
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
  const float4x4 rotTm = math::rotateZYX(math::radians(roll), math::radians(pitch), math::radians(yaw));

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