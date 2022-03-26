#include "archetype.h"
#include "components_accessor.h"

#include <engine/assert.h>
#include <engine/platform/memory.h>

namespace Engine::ECS
{
  bool ComponentsStorage::addEntity(const uint8_t* data, const size_t nComps, const uint16_t* offsets, const uint16_t* sizes, chunk_id& chunkId, block_id& blockId)
  {
    if (!getFreeChunk(chunkId, blockId))
      return false;
  
    uint8_t* block = m_Chunks[chunkId].data + blockId * m_BlockSize;
    for(size_t i = 0; i < nComps; ++i)
    {
      uint8_t* comp = block + offsets[i];
      const uint8_t* dataComp = data + offsets[i];
      memcpy(comp, dataComp, sizes[i]);
    }
  
    return true;
  }
  
  void ComponentsStorage::destroyEntity(const ComponentMap& compMap, const chunk_id chunkId, const block_id blockId, block_id& replacedBlockId)
  {
    Chunk& chunk = m_Chunks[chunkId];
  
    uint8_t* destroingData = chunk.data + blockId * m_BlockSize;
    EntityDestroyer{compMap}.destroyEntity(destroingData);
  
    const block_id lastBlockInUse = chunk.usedBlocks;
  
    if (blockId == lastBlockInUse)
    {
      chunk.usedBlocks -= 1;
      return;
    }
  
    replacedBlockId = lastBlockInUse;
    uint8_t* replacingData =  chunk.data + replacedBlockId * m_BlockSize;
  
    memcpy(destroingData, replacingData, m_BlockSize);
    chunk.usedBlocks -=1 ;
  }
  
  bool ComponentsStorage::getFreeChunk(chunk_id& chunkId, block_id& blockId)
  {
    for(size_t i = 0; i < m_Chunks.size(); ++i)
    {
      Chunk& chunk = m_Chunks[i];
      if (chunk.usedBlocks < chunk.capacity)
      {
        chunkId = (chunk_id)i;
        blockId = chunk.usedBlocks;
        chunk.usedBlocks += 1;
        return true;
      }
    }
  
    Chunk& chunk = m_Chunks.push_back();
    chunk.allocate(CHUNK_SIZE, m_BlockSize);
    chunk.usedBlocks += 1;
    chunkId = 0;
    blockId = 0;
    return true;
  }
  
  Archetype::Archetype(const eastl::vector<ComponentDescription>& desc)
  {
    m_ComponentsMap.reserve(desc.size());
  
    size_t blockOffset = 0;
    for(const auto& d: desc)
    {
      m_ComponentsMap.insert({
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
  
    const size_t blockSize = blockOffset;
    m_CompStorage.init(blockSize);
  }
  
  bool Archetype::hasComponents(const eastl::vector<ComponentDescription>& desc) const
  {
    for(const auto& comp: desc)
    {
      if (m_ComponentsMap.find(comp.id) == m_ComponentsMap.end())
        return false;
    }
    return true;
  }
  
  bool Archetype::hasComponent(const component_type_id typeId, const component_name_id nameId) const
  {
    const auto it = m_ComponentsMap.find(nameId);
    if (it != m_ComponentsMap.end() && it->second.typeId == typeId)
      return true;
  
    return false;
  }

}
