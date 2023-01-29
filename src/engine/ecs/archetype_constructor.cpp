#include "archetype_constructor.h"
#include "archetype.h"
#include "archetype_searcher.h"
#include "archetype_storage.h"

#include <engine/platform/memory.h>

#include <ranges>

namespace Engine::ECS
{
  ArchetypeConstructor::ArchetypeConstructor(const ArchetypeSearcher& searcher,
                                             eastl::vector<ArchetypeStorage>& archetypes)
    : m_ArchetypeSearcher(searcher)
    , m_Archetypes(archetypes)
  {
  }

  ComponentMap ArchetypeConstructor::constructComponentMap(const ArchetypeComponentsDescription& desc,
                                                           size_t& block_size) const
  {
    ComponentMap compMap;
    compMap.reserve(desc.components.size());
  
    size_t blockOffset = 0;
    for(const auto& d: desc.components)
    {
      compMap.insert({
        d.id,
        Component{
          .size = d.size,
          .blockOffset = blockOffset,
          .typeId = d.type
        }
      });
  
      blockOffset += d.size;
      Sys::align(blockOffset);
    }
  
    block_size = blockOffset;
    return compMap;
  }

  archetype_id ArchetypeConstructor::addArchetype(ArchetypeComponentsDescription&& desc)
  {
    size_t blockSize = 0;
    ComponentMap compMap = constructComponentMap(desc, blockSize);

    const archetype_id archetypeId = (archetype_id)m_Archetypes.size();
    m_Archetypes.emplace_back(blockSize, std::move(desc), std::move(compMap));

    return archetypeId;
  }

  archetype_id ArchetypeConstructor::operator()(eastl::vector<ComponentDescription>&& components,
                                                const eastl::vector<archetype_id>& extends,
                                                const eastl::vector<string>& extend_names)
  {

    ArchetypeComponentsDescription desc{std::move(components)};
    
    bool archetypeValid = true;
    for (size_t i = 0; const archetype_id archId: extends)
    {
      if (!desc.merge(m_Archetypes[archId].getDescription()))
      {
        logerror("Failed to extend `{}` see previous errors", extend_names[i]);
        archetypeValid = false;
      }
      ++i;
    }

    if (!archetypeValid)
      return INVALID_ARCHETYPE_ID;

    archetype_id archetypeId = m_ArchetypeSearcher.find(desc);
    if (archetypeId != INVALID_ARCHETYPE_ID)
      return archetypeId;

    return addArchetype(std::move(desc));
  }

  archetype_id ArchetypeConstructor::operator()(const eastl::vector<archetype_id>& archetypes,
                                                const eastl::vector<string>& template_names)
  {
    ArchetypeComponentsDescription newArchDesc;

    bool isValidArch = true;
    for (size_t i = 0; archetype_id archId : archetypes)
    {
      if (archId == INVALID_ARCHETYPE_ID)
      {
        logerror("invalid template `{}`: it's not registered", template_names[i]);
        isValidArch = false;
      }
      ++i;

      newArchDesc.merge(m_Archetypes[archId].getDescription());
    }

    return addArchetype(std::move(newArchDesc));
  }
}
