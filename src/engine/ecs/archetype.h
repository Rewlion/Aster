#pragma once

#include "types.h"
#include "entity_initializer.h"
#include "query.h"

#include <engine/types.h>

namespace Engine::ECS
{
  struct Chunk
  {
    inline void reset()
    {
      if (data)
      {
        delete[] data;
        data = nullptr;
      }
      capacity = 0;
      usedBlocks = 0;
    }

    inline ~Chunk()
    {
      reset();
    }

    inline void allocate(const size_t size, const size_t blockSize)
    {
      data = new uint8_t[size];
      capacity = (uint16_t)(size / blockSize);
      usedBlocks = 0;
    }

    uint8_t* data = nullptr;
    uint16_t capacity = 0;
    uint16_t usedBlocks = 0;
  };

  class ComponentsStorage
  {
    friend class Registry;

    public:
      inline ComponentsStorage()
        : m_BlockSize(0)
      {
      }

      bool addEntity(const uint8_t* data, const size_t nComps, const uint16_t* offsets, const uint16_t* sizes, chunk_id& chunkId, block_id& blockId);

      void destroyEntity(const ComponentMap& compMap, const chunk_id chunkId, const block_id blockId, block_id& replacedBlockId);

      inline void init(const size_t blockSize)
      {
        m_Chunks.clear();
        m_BlockSize = blockSize;
      }

      inline size_t getBlockSize() const
      {
        return m_BlockSize;
      }

    private:
      bool getFreeChunk(chunk_id& chunkId, block_id& blockId);

    private:
      eastl::fixed_vector<Chunk, 16, true> m_Chunks;
      size_t m_BlockSize;
  };

  class Registry;
  class Archetype
  {
    friend Registry;

    public:
      Archetype(const eastl::vector<ComponentDescription>& desc);

      bool hasComponents(const eastl::vector<ComponentDescription>& desc) const;

      bool hasComponent(const component_type_id typeId, const component_name_id nameId) const;

      inline const ComponentMap getComponentMap() const
      {
        return m_ComponentsMap;
      }

      inline EntityInitializer getNewEntityInitializer() const
      {
        return EntityInitializer(m_ComponentsMap, m_CompStorage.getBlockSize());
      }

    private:
      ComponentMap m_ComponentsMap;
      ComponentsStorage m_CompStorage;
  };

}