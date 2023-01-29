#pragma once

#include "types.h"
#include "components.h"

#include <engine/types.h>

#include <EASTL/vector.h>

namespace Engine::ECS
{
  struct ArchetypeComponentsDescription;
  class ArchetypeStorage;
  class ArchetypeSearcher;
  struct ComponentDescription;

  class ArchetypeConstructor
  {
    public:
      ArchetypeConstructor(const ArchetypeSearcher&, 
                           eastl::vector<ArchetypeStorage>&);

      archetype_id operator()(eastl::vector<ComponentDescription>&&,
                              const eastl::vector<archetype_id>& extends,
                              const eastl::vector<string>& extend_names);

      archetype_id operator()(const eastl::vector<archetype_id>& archetypes,
                              const eastl::vector<string>& template_names);
    private:
      ComponentMap constructComponentMap(const ArchetypeComponentsDescription& desc, size_t& block_size) const;
      archetype_id addArchetype(ArchetypeComponentsDescription&&);

    private:
      const ArchetypeSearcher& m_ArchetypeSearcher;
      eastl::vector<ArchetypeStorage>& m_Archetypes;
  };
}
