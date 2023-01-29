#include "archetype_searcher.h"
#include "archetype.h"

#include <engine/algorithm/hash.h>

namespace Engine::ECS
{
  ArchetypeSearcher::ArchetypeSearcher(const TemplateToArchetypeMap& map, const eastl::vector<ArchetypeStorage>& archetypes)
    : m_TemplateToArhetypeMap(map)
    , m_Archetypes(archetypes)
  {
  }

  archetype_id ArchetypeSearcher::find(const string_view& template_name) const
  {
    const template_name_id tmplId = str_hash(template_name);

    const auto archIt = m_TemplateToArhetypeMap.find(tmplId);

    if (archIt != m_TemplateToArhetypeMap.end())
      return archIt->second;

    return INVALID_ARCHETYPE_ID;
  }

  archetype_id ArchetypeSearcher::find(const ArchetypeComponentsDescription& description) const
  {
    for (size_t i = 0; i < m_Archetypes.size(); ++i)
     if (m_Archetypes[i].hasComponents(description.components))
       return (archetype_id)i;

    return INVALID_ARCHETYPE_ID;
  }


}
