#pragma once

#include "transform.h"

namespace ed
{
  class Scope;
}

class CameraComponent : public TransformComponent
{
  public:
    CameraComponent() = default;
    CameraComponent(const ed::Scope*);
    CameraComponent(const bool is_active, const float fov,
                    const float znear, const float zfar,
                    const float3& pos, const float3 rot);

    void setFOV(const float fov) { m_FOV = fov; }
    auto getFOV() const -> float { return m_FOV; }

    void setZNear(const float znear) { m_ZNear = znear; }
    auto getZNear() const -> float { return m_ZNear; }

    void setZFar(const float zfar) { m_ZFar = zfar; }
    auto getZFar() const -> float { return m_ZFar; }

    auto toggleActive(const bool v) -> bool
    {
      const bool old = m_IsActive; 
      m_IsActive = v; 
      return old;
    }
    auto isActive() const -> bool { return m_IsActive; }

  private:
    bool m_IsActive = false;
    float m_FOV = 90.0f;
    float m_ZNear = 0.1f;
    float m_ZFar = 500.0f;
};