#include "entity_component_init.h"

namespace ecs
{
  auto EntityComponents::operator[](const hashed_name name) -> TemplateComponent&
  {
    m_Inits.push_back();
    m_Inits.back() = EntityComponentInit{name, TemplateComponent{}};
    return m_Inits.back().component;
  }

  auto EntityComponents::getInits() const -> const eastl::vector<EntityComponentInit>&
  {
    return m_Inits;
  }
}
