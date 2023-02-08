#include "entity_components.h"

namespace Engine::ECS
{
  EntityComponents::EntityComponents(const size_t block_size, 
                                     const eastl::vector<ArchetypeComponent>& comps,
                                     const CompToPlacementMap& placement_map)
    : m_CompToPlacementsMap(placement_map)
  {
    const size_t N = placement_map.size();
    const size_t metaStart = sizeof(*m_BlockOffsets) * N;
    const size_t dataStart = metaStart + sizeof(void*) * N;
    const size_t bufferSize = dataStart + block_size;

    m_Buffer = new std::byte[bufferSize];
    m_BlockOffsets = (size_t*)m_Buffer;
    m_Metas = (const ComponentMeta**)(m_Buffer + metaStart);
    m_Data = m_Buffer + dataStart;
    m_N = N;

    for (size_t i = 0; const auto& comp: comps)
    {
      const ComponentMeta& meta = get_meta_storage().getMeta(comp.typeId);
      m_BlockOffsets[i] = comp.blockOffset;
      m_Metas[i] = &meta;
      
      std::byte* compData = m_Data + comp.blockOffset;
      if (meta.isTrivial)
        std::memset(compData, 0, meta.size);
      else
        meta.manager->constructor(compData);

      ++i;
    }
  }

  EntityComponents::~EntityComponents()
  {
    if (m_Buffer)
    {
      delete[] m_Buffer;
      m_Buffer = nullptr;
      m_BlockOffsets = nullptr;
      m_Metas = nullptr;
      m_Data = nullptr;
      m_N = 0;
    }
  }

  EntityComponents::EntityComponents(EntityComponents&& rvl)
    : m_CompToPlacementsMap(rvl.m_CompToPlacementsMap)
  {
    this->~EntityComponents();
    std::swap(m_Buffer, rvl.m_Buffer);
    std::swap(m_BlockOffsets, rvl.m_BlockOffsets);
    std::swap(m_Metas, rvl.m_Metas);
    std::swap(m_Data, rvl.m_Data);
    std::swap(m_N, rvl.m_N);
  }

  
  ComponentsInitializer EntityComponents::operator[] (const component_name_id nameId)
  {
    const auto it = m_CompToPlacementsMap.find(nameId);
    ASSERT_FMT(it != m_CompToPlacementsMap.end(), "ecs: entity doesn't have such component");
    
    std::byte* data = m_Data + m_BlockOffsets[it->second];
    const ComponentMeta& meta = *m_Metas[it->second];

    return ComponentsInitializer{ data, meta };
  }

  void EntityComponents::moveData(std::byte* to)
  {
    for (size_t i = 0; i < m_N; ++i)
    {
      std::byte* toData = to + m_BlockOffsets[i];
      std::byte* fromData = m_Data + m_BlockOffsets[i];
      const ComponentMeta& meta = *m_Metas[i];

      if (meta.isTrivial || meta.isTrivialRelocatable)
        std::memcpy(toData, fromData, meta.size);
      else
        m_Metas[i]->manager->move(toData, fromData);
    }
  }
}
