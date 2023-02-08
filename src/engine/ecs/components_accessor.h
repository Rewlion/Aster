#pragma once

#include "components.h"

#include <engine/assert.h>

namespace Engine::ECS
{
  class ComponentsAccessor
  {
    public:
      inline ComponentsAccessor(std::byte* compData,
                                const CompToPlacementMap& comp_to_placement_map,
                                const eastl::vector<ArchetypeComponent>& comps)
        : m_ComponentsData(compData)
        , m_CompToPlacementMap(comp_to_placement_map)
        , m_Components(comps)
      {
      }

      template<class T>
      T& get(const component_name_id component)
      {
        const auto it = m_CompToPlacementMap.find(component);
        ASSERT(it != m_CompToPlacementMap.end());

        const size_t blockOffset = m_Components[it->second].blockOffset;
        return reinterpret_cast<T&>(*(m_ComponentsData + blockOffset));
      }

      template<class T>
      T& get(const component_name_id component, T& defValue)
      {
         const auto it = m_CompToPlacementMap.find(component);
        if (it != m_CompToPlacementMap.end())
        {
          const size_t blockOffset = m_Components[it->second].blockOffset;
          return reinterpret_cast<T&>(*(m_ComponentsData + blockOffset));
        }

        return defValue;
      }

    private:
      std::byte* m_ComponentsData;
      const CompToPlacementMap& m_CompToPlacementMap;
      const eastl::vector<ArchetypeComponent>& m_Components;
  };
}