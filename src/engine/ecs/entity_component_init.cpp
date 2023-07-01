#include "entity_component_init.h"

namespace ecs
{
  auto EntityComponents::operator[](const char* name) -> TemplateComponent&
  {
    
    m_Inits.push_back();
    m_Inits.back() = EntityComponentInit{hashed_name(string_view{name}), TemplateComponent{}};
    return m_Inits.back().component;
  }

  auto EntityComponents::getInits() -> eastl::vector<EntityComponentInit>&
  {
    return m_Inits;
  }
}
