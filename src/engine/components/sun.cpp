#include "sun.h"

#include <engine/data/ed.h>
#include <engine/ecs/type_meta.h>

SunComponent::SunComponent(const ed::Scope* init)
{
  m_Azimuth = init->getVariableOr("azimuth", 0.0f);
  m_Altitude = init->getVariableOr("altitude", 0.0f);
}

void SunComponent::updateOrientation(const float azimuth, const float altitude)
{
  m_Azimuth = azimuth;
  m_Altitude = altitude;
}

DECLARE_TRIVIAL_ECS_COMPONENT(SunComponent);
