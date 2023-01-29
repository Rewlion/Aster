#pragma once

#include "types.h"

#include <engine/types.h>

namespace Engine::ECS
{
  class ArchetypeStorage;
  struct ArchetypeComponentsDescription;

  class ArchetypeSearcher
  {
    public:
      ArchetypeSearcher(const TemplateToArchetypeMap&, const eastl::vector<ArchetypeStorage>&);

      archetype_id find(const string_view& template_name) const;
      archetype_id find(const ArchetypeComponentsDescription&) const;

    private:
      const TemplateToArchetypeMap& m_TemplateToArhetypeMap;
      const eastl::vector<ArchetypeStorage>& m_Archetypes;
  };
}
