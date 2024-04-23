#pragma once

#include "transform.h"

class StaticMeshComponent : public TransformComponent
{
  public:
    StaticMeshComponent() = default;
    StaticMeshComponent(const ed::Scope*);

    auto getModelName() const -> string_view { return m_Model; }

  private:
    string m_Model;
};