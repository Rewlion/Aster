#pragma once

#include "eid.h"
#include "entity_cb.h"
#include "entity_components.h"
#include "query.h"
#include "types.h"

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
      if (blocksOwners)
      {
        delete[] blocksOwners;
        blocksOwners = nullptr;
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
      blocksOwners = new EntityId[capacity];
      std::memset(blocksOwners, 0, sizeof(*blocksOwners) * capacity);
      usedBlocks = 0;
    }

    uint8_t* data = nullptr;
    EntityId* blocksOwners = nullptr;
    uint16_t capacity = 0;
    uint16_t usedBlocks = 0;
  };

  struct ChunkInfo
  {
    chunk_id chunkId;
    block_id blockId;
  };

  struct DestroidEntityInfo
  {
    block_id replacedBlockId;
    EntityId replacedBlockOwner;
  };

  class ComponentsAccessor;
  class ArchetypeStorage
  {
    friend class Registry;

    public:
      using ForEachCb = eastl::function<void(ComponentsAccessor&)>;

    public:
      inline ArchetypeStorage(const size_t blockSize, 
                              eastl::vector<ArchetypeComponent>&& comps,
                              CompToPlacementMap&& placements)
        : m_BlockSize(blockSize)
        , m_Components(std::move(comps))
        , m_CompToPlacementsMap(placements)
      {
      }

      ChunkInfo addEntity(EntityId, EntityComponents&&);

      DestroidEntityInfo destroyEntity(const chunk_id chunkId, const block_id blockId);

      inline size_t getBlockSize() const
      {
        return m_BlockSize;
      }

      inline const eastl::vector<ArchetypeComponent>& getComponents() const
      {
        return m_Components;
      }

      void forEachEntity(const ForEachCb& cb);

      bool hasComponents(const eastl::vector<ArchetypeComponent>&) const;

      const CompToPlacementMap& getCompToPlacementsMap() const
      {
        return m_CompToPlacementsMap;
      }

    private:
      chunk_id getFreeChunkId();
      ChunkInfo getFreeChunk(const EntityId eid);
      void destroyEntityData(std::byte* data);

    private:
      eastl::vector<Chunk> m_Chunks;
      size_t m_BlockSize;
      
      eastl::vector<ArchetypeComponent> m_Components;
      CompToPlacementMap m_CompToPlacementsMap;
  };
}
