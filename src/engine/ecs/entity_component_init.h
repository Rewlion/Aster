#pragma once

#include "template_component.h"
#include "hashed_string.h"

namespace ecs
{
  struct EntityComponentInit
  {
    hashed_name name;
    TemplateComponent component;
  };

  class EntityComponents
  {
    friend class Registry;
    public:
      auto operator[](const hashed_name name) -> TemplateComponent&;

    private:
      auto getInits() const -> const eastl::vector<EntityComponentInit>&;

    private:
      eastl::vector<EntityComponentInit> m_Inits;
  };
}
