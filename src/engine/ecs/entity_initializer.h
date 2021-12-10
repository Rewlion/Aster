#pragma once

#include "components.h"
#include "types.h"

#include <engine/types.h>
#include <engine/assert.h>

#include <EASTL/fixed_vector.h>

class EntityDestroyer
{
  public:
    inline EntityDestroyer(const ComponentMap& compMap)
      : m_ComponentsMap(compMap)
    {
    }

    inline void destroy_entity(uint8_t* data)
    {
      for(auto& [_, comp]: m_ComponentsMap)
      {
        uint8_t* compData = data + comp.blockOffset;
        component_destructor(data, comp.typeId);
      }
    }

  private:
    const ComponentMap& m_ComponentsMap;
};

class EntityInitializer
{
  friend class Registry;
  public:
    inline EntityInitializer(const ComponentMap& compMap, const size_t blockSize)
      : m_ComponentMap(compMap)
    {
      const size_t componentsCount = compMap.size();
      const size_t offsetsStart = blockSize;
      const size_t sizesStart = offsetsStart + componentsCount * sizeof(*m_Offsets);
      const size_t typesStart = sizesStart + componentsCount * sizeof(*m_Sizes);
      const size_t initializeBufferSize = typesStart + componentsCount * sizeof(*m_Types);

      m_Buffer = new uint8_t[initializeBufferSize];
      m_Data = m_Buffer;
      m_Offsets = reinterpret_cast<uint16_t*>(m_Data + offsetsStart);
      m_Sizes = reinterpret_cast<uint16_t*>(m_Data + sizesStart);
      m_Types = reinterpret_cast<component_type_id*>(m_Data + typesStart);

      int i = 0;
      for(auto &[_, comp]: compMap)
      {
        uint8_t* component_data = m_Data + comp.blockOffset;
        component_default_initializer(component_data, comp.typeId);
        m_Offsets[i] = (uint16_t)comp.blockOffset;
        m_Sizes[i] = (uint16_t)comp.size;
        m_Types[i] = comp.typeId;
        ++i;
      }
    }

    inline EntityInitializer(EntityInitializer&& r)
      : m_ComponentMap(r.m_ComponentMap)
    {
      this->~EntityInitializer();
      m_Buffer = r.m_Buffer; r.m_Buffer = nullptr;
      m_Data = r.m_Data; r.m_Data = nullptr;
      m_Offsets = r.m_Offsets; r.m_Offsets = nullptr;
      m_Sizes = r.m_Sizes; r.m_Sizes = nullptr;
      m_Types = r.m_Types; r.m_Types = nullptr;
    }

    inline ~EntityInitializer()
    {
      if (m_Buffer)
        delete[] m_Buffer;
    }

    EntityInitializer& operator=(const EntityInitializer&) = delete;

    template<class T>
    void init_component(const component_name_id nameId, T&& value)
    {
      const component_type_id typeId = get_component_type_id<T>();
      const auto it = m_ComponentMap.find(nameId);
      ASSERT(it != m_ComponentMap.end());
      ASSERT(it->second.typeId == typeId);

      T* compData = reinterpret_cast<T*>(m_Data + it->second.blockOffset);
      *compData = std::move(value);
    }

  private:
    const ComponentMap& m_ComponentMap;

    uint8_t* m_Buffer;
    uint8_t* m_Data;
    uint16_t* m_Offsets;
    uint16_t* m_Sizes;
    component_type_id* m_Types;
};
