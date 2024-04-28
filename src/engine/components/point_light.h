#pragma once

#include "transform.h"

namespace ed
{
  class Scope;
}

class PointLightComponent :  public TransformComponent
{
  public:
    PointLightComponent() = default;
    PointLightComponent(const ed::Scope*);
    PointLightComponent(const float attenuation_radius, const float3& color, const float3& pos);

    auto getAttenuationRadius() const -> float { return m_AttenuationRadius; }
    auto getColor() const -> float3 { return m_Color; }

  private:
    float m_AttenuationRadius;
    float3 m_Color;
};
