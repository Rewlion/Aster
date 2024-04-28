#pragma once

#include "transform.h"

namespace ed
{
  class Scope;
}

class DecalComponent : public TransformComponent
{
  public:
    DecalComponent() = default;
    DecalComponent(const ed::Scope*);
    DecalComponent(const string_view decal_name, const float3& pos, const float3& scale);

    auto getName() const -> string_view { return m_Name; }

  private:
    string m_Name;
};