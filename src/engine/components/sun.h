#pragma once

namespace ed
{
  class Scope;
}

class SunComponent
{
  public:
    SunComponent() = default;
    SunComponent(const ed::Scope*);

    auto getAzimuth() const -> float { return m_Azimuth; }
    auto getAltitude() const -> float { return m_Altitude; }

    void updateOrientation(const float azimuth, const float altitude);

  private:
    float m_Azimuth = 0.0f;
    float m_Altitude = 0.0f;
};
