#pragma once

#include "transform.h"

class StaticMeshComponent : public TransformComponent
{
  public:
    StaticMeshComponent() = default;
    StaticMeshComponent(const ed::Scope*);

    auto getModelName() const -> string_view { return m_Model; }
    void setModelName(const string& name) { m_Model = name; }

  private:
    string m_Model;
};