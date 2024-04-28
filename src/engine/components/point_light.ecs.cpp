#include "point_light.h"

#include <engine/data/ed.h>
#include <engine/ecs/macros.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>

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

ECS_EVENT_SYSTEM()
static
void default_init_point_light_component(const ecs::OnEntityCreated& evt,
                                        const TransformComponent& root_tm,
                                        PointLightComponent& point_light)
{
  if (!point_light.hasAttachment())
    point_light.attachToSelfComponent(&root_tm);
}

DECLARE_TRIVIAL_ECS_COMPONENT(PointLightComponent);