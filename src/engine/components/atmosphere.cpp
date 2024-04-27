#include "atmosphere.h"

#include <engine/ecs/type_meta.h>
#include <engine/data/ed.h>

AtmosphereComponent::AtmosphereComponent(const ed::Scope* init)
{
  m_TopRadius_km = init->getVariableOr("top_radius_km", 6420);
  m_BotRadius_km = init->getVariableOr("bot_radius_km", 6360);
}

DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(AtmosphereComponent);
