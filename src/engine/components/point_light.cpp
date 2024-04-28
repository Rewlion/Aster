#include "point_light.h"

#include <engine/data/ed.h>
#include <engine/ecs/type_meta.h>

PointLightComponent::PointLightComponent(const ed::Scope* init)
  : TransformComponent(init)
{
  m_AttenuationRadius = init->getVariableOr("attenuation_radius", 1.0f);
  m_Color = init->getVariableOr("color", float3(1,1,1));
}

PointLightComponent::PointLightComponent(const float attenuation_radius, const float3& color, const float3& pos)
  : TransformComponent(pos, float3(0,0,0), float3(1,1,1))
  , m_AttenuationRadius(attenuation_radius)
  , m_Color(color)
{
}

DECLARE_TRIVIAL_ECS_COMPONENT(PointLightComponent);