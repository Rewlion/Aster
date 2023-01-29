#include "archetype.h"
#include "components_accessor.h"
#include "entity_cb.h"

#include <engine/assert.h>
#include <engine/platform/memory.h>

namespace Engine::ECS
{
  bool ArchetypeStorage::hasComponents(const eastl::vector<ComponentDescription>& components) const
  {
    for(const auto& comp: components)
    {
      if (m_ComponentsMap.find(comp.id) == m_ComponentsMap.end())
        return false;
    }
    return true;
  }
  
  bool ArchetypeStorage::hasComponent(const component_type_id typeId, const component_name_id nameId) const
  {
    const auto it = m_ComponentsMap.find(nameId);
    if (it != m_ComponentsMap.end() && it->second.typeId == typeId)
      return true;
  
    return false;
  }

  bool ArchetypeComponentsDescription::merge(const ArchetypeComponentsDescription& rvl)
  {
    eastl::vector<ComponentDescription> newComponents;
    newComponents.reserve(rvl.components.size());

    bool isValid = true;
    for (const auto& comp: rvl.components)
    {
      const auto it = eastl::find_if(components.begin(),
                                     components.end(),
                                     [&comp](const ComponentDescription& lvl){ return lvl.id == comp.id; });
      if (it != components.end())
      {
        if (comp.type != it->type)
        {
          logerror("failed to overwrite a component `{}`: types are not equal", comp.name);
          isValid = false;
        }
      }
      else
      {
        newComponents.push_back(comp);
      }
    }

    if (!isValid)
      return false;

    for (const auto c: newComponents)
      components.push_back(c);

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

  ChunkInfo ArchetypeStorage::addEntity(EntityId eid, const CreationCb& cb)
  {
    EntityInitializer init{m_ComponentsMap, m_BlockSize};
    cb(eid, init);

    ChunkInfo chunkInfo = getFreeChunk(eid);
  
    uint8_t* block = m_Chunks[chunkInfo.chunkId].data + chunkInfo.blockId * m_BlockSize;
    for(size_t i = 0; i < m_ComponentsMap.size(); ++i)
    {
      uint8_t* comp = block + init.m_Offsets[i];
      const uint8_t* dataComp = init.m_Data + init.m_Offsets[i];
      memcpy(comp, dataComp, init.m_Sizes[i]);
    }

    return chunkInfo;
  }
  
  DestroidEntityInfo ArchetypeStorage::destroyEntity(const chunk_id chunkId, const block_id blockId)
  {
    DestroidEntityInfo dstrInfo;
    Chunk& chunk = m_Chunks[chunkId];

    uint8_t* destroingData = chunk.data + blockId * m_BlockSize;
    EntityDestroyer{m_ComponentsMap}.destroyEntity(destroingData);
  
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
        uint8_t* data = chunk.data + m_BlockSize * i;
        ComponentsAccessor compAccessor(data, m_ComponentsMap);
        cb(compAccessor);
      }
    }
  }
}
