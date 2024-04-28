#include "sun.h"

#include <engine/data/ed.h>
#include <engine/ecs/ecs.h>
#include <engine/ecs/type_meta.h>
#include <engine/events.h>

SunComponent::SunComponent(const ed::Scope* init)
{
  m_Azimuth = init->getVariableOr("azimuth", 0.0f);
  m_Altitude = init->getVariableOr("altitude", 0.0f);
}

void SunComponent::updateOrientation(const float azimuth, const float altitude)
{
  const bool changed = azimuth != m_Azimuth || altitude != m_Altitude;
  m_Azimuth = azimuth;
  m_Altitude = altitude;

  if (changed)
  {
    Engine::SunOrientationChanged evt;
    evt.newAzimuth = m_Azimuth;
    evt.newAltitude = m_Altitude;

    ecs::get_registry().broadcastEvent(std::move(evt));
  }
}

DECLARE_TRIVIAL_ECS_COMPONENT(SunComponent);
