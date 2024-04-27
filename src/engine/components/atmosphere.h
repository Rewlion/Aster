#pragma once

namespace ed
{
  class Scope;
}

class AtmosphereComponent
{
  public:
    AtmosphereComponent() = default;
    AtmosphereComponent(const ed::Scope*);

    auto getTopRadiusKm() const -> float { return m_TopRadius_km; }
    auto getBotRadiusKm() const -> float { return m_BotRadius_km; }

  private:
    float m_TopRadius_km;
    float m_BotRadius_km;
};