#pragma once

#include "components.h"

#include <engine/assert.h>

namespace Engine::ECS
{
  class ComponentsAccessor
  {
    public:
      inline ComponentsAccessor(uint8_t* compData, const ComponentMap& compMap)
        : m_ComponentsData(compData)
        , m_ComponentMap(compMap)
      {
      }

      template<class T>
      T& Get(const component_name_id component)
      {
        const auto it = m_ComponentMap.find(component);
        ASSERT(it != m_ComponentMap.end());

        const component_type_id requestedType = get_component_type_id<T>();
        ASSERT(requestedType != INVALID_COMPONENT_TYPE_ID);
        ASSERT(requestedType == it->second.typeId);

        return reinterpret_cast<T&>(*(m_ComponentsData + it->second.blockOffset));
      }

      template<class T>
      T& Get(const component_name_id component, T& defValue)
      {
        const auto it = m_ComponentMap.find(it);
        if (it != m_ComponentMap.end())
        {
          const component_type_id requestedType = get_component_type_id<T>();
          if (requestedType != INVALID_COMPONENT_TYPE_ID && requestedType == it->second.typeId)
            return reinterpret_cast<T&>(*(m_ComponentsData + it->second.blockOffset));
        }

        return defValue;
      }

    private:
      uint8_t* m_ComponentsData;
      const ComponentMap& m_ComponentMap;
  };
}