#pragma once

#include <engine/types.h>
#include <engine/ecs/eid.h>

#include <variant>

namespace ed
{
  class Scope;
}

class TransformComponent
{
  public:
    TransformComponent() = default;
    
    TransformComponent(const float3& pos, const float3& rot, const float3& scale)
      : m_LocalPosition(pos)
      , m_LocalRotation(rot)
      , m_LocalScale(scale)
    {
    }

    TransformComponent(const ed::Scope*);

    void attachToEntity(const ecs::EntityId, const TransformComponent* entity_comp);
    void attachToSelfComponent(const TransformComponent*);
    auto hasAttachment() const -> bool { return m_ParentAttachment.index() != 0; }

    auto getLocalPosition() const -> float3 { return m_LocalPosition; }
    auto getWorldPosition() const -> float3;
    auto getLocalRotation() const -> float3 { return m_LocalRotation; }
    
    auto getWorldTransform() const -> float4x4;
    auto getLocalTransform() const -> float4x4;

    void setLocalPosition(const float3& pos) { m_LocalPosition = pos; }
    void setLocalRotation(const float3& rot) { m_LocalRotation = rot; }
    void setLocalScale(const float3& scale) { m_LocalScale = scale; }

    auto getWorldForward() const -> float3;

  protected:
    struct SelfAttachment
    {
      const TransformComponent* comp;
    };
    struct EntityAttachment
    {
      ecs::EntityId eid;
      const TransformComponent* comp;
    };
    
    std::variant<std::monostate,
                 SelfAttachment,
                 EntityAttachment> m_ParentAttachment;

    float3 m_LocalPosition;
    float3 m_LocalRotation;
    float3 m_LocalScale;

  private:
    auto getWorldTransformTmInternal(const TransformComponent*) const -> float4x4;
    auto getParentTransform() const -> const TransformComponent*;
};