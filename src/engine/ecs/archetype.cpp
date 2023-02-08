#include "archetype.h"
#include "components_accessor.h"
#include "entity_cb.h"

#include <engine/assert.h>
#include <engine/platform/memory.h>

namespace Engine::ECS
{
  bool ArchetypeStorage::hasComponents(const eastl::vector<ArchetypeComponent>& comps) const
  {
    for(const auto& c: comps)
    {
      const auto it = m_CompToPlacementsMap.find(c.nameId);
      if (it != m_CompToPlacementsMap.end())
      {
        const auto thisType = m_Components[it->second];
        if (c.typeId != thisType.typeId)
          return false;
      }
      else
        return false;
    }
    return true;
  }

  chunk_id ArchetypeStorage::getFreeChunkId()
  {
    for(size_t i = 0; i < m_Chunks.size(); ++i)
    {
      Chunk& chunk = m_Chunks[i];
      if (chunk.usedBlocks < chunk.capacity)
        return i;
    }
  
    Chunk& chunk = m_Chunks.push_back();
    chunk.allocate(CHUNK_SIZE, m_BlockSize);

    return m_Chunks.size()-1;
  }

  ChunkInfo ArchetypeStorage::getFreeChunk(const EntityId eid)
  {
    const chunk_id freeChunkId = getFreeChunkId();

    Chunk& chunk = m_Chunks[freeChunkId];
    const block_id freeBlockId = chunk.usedBlocks;
    
    ChunkInfo chunkInfo {
      .chunkId = freeChunkId,
      .blockId = freeBlockId
    };

    chunk.blocksOwners[freeBlockId] = eid;
    chunk.usedBlocks += 1;

    return chunkInfo;
  }

  ChunkInfo ArchetypeStorage::addEntity(EntityId eid, EntityComponents&& entity_comps)
  {
    ChunkInfo chunkInfo = getFreeChunk(eid);
  
    uint8_t* block = m_Chunks[chunkInfo.chunkId].data + chunkInfo.blockId * m_BlockSize;
    entity_comps.moveData((std::byte*)block);

    return chunkInfo;
  }
  
  void ArchetypeStorage::destroyEntityData(std::byte* data)
  {
    for(const auto& comp: m_Components)
    {
      const size_t blockOffset = comp.blockOffset;
      std::byte* compData = data + blockOffset;
      const ComponentMeta& meta = get_meta_storage().getMeta(comp.typeId);
      if (!meta.isTrivial)
        meta.manager->destructor(compData);
    }
  }

  DestroidEntityInfo ArchetypeStorage::destroyEntity(const chunk_id chunkId, const block_id blockId)
  {
    DestroidEntityInfo dstrInfo;
    Chunk& chunk = m_Chunks[chunkId];

    std::byte* destroingData = (std::byte*)(chunk.data + blockId * m_BlockSize);
    destroyEntityData(destroingData);
  
    const block_id lastBlockInUse = chunk.usedBlocks;
  
    if (blockId == lastBlockInUse)
    {
      chunk.usedBlocks -= 1;
      chunk.blocksOwners[blockId] = {};
      dstrInfo.replacedBlockId = INVALID_BLOCK_ID;
      dstrInfo.replacedBlockOwner = {};
      return dstrInfo;
    }
  
    const block_id replacedBlockId = lastBlockInUse;
    EntityId& replacedBlockOwner = chunk.blocksOwners[replacedBlockId];

    uint8_t* replacingData = chunk.data + replacedBlockId * m_BlockSize;
    memcpy(destroingData, replacingData, m_BlockSize);
    chunk.blocksOwners[blockId] = replacedBlockOwner;
    
    dstrInfo.replacedBlockId = replacedBlockId;
    dstrInfo.replacedBlockOwner = replacedBlockOwner;

    replacedBlockOwner = {};
    chunk.usedBlocks -= 1;

    return dstrInfo;
  }

  void ArchetypeStorage::forEachEntity(const ForEachCb& cb)
  {
    for(Chunk& chunk: m_Chunks)
    {
      for (int i = 0; i < chunk.usedBlocks; ++i)
      {
        std::byte* compData = (std::byte*)(chunk.data + m_BlockSize * i);
        ComponentsAccessor compAccessor{compData, m_CompToPlacementsMap, m_Components };
        cb(compAccessor);
      }
    }
  }
}
