#include "entity_storage.h"

constexpr size_t MAX_CHUNK_SIZE = 4 * 1024;

namespace ecs
{
  void EntityStorage::Chunk::reset()
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

  EntityStorage::Chunk::Chunk(const size_t entity_size, const size_t capacity)
  {
    data = new std::byte[entity_size * capacity];
    std::memset(data, 0, entity_size * capacity);
    this->capacity = capacity;
    blocksOwners = new EntityId[capacity];
    std::memset(blocksOwners, 0, sizeof(*blocksOwners) * capacity);
    usedBlocks = 0;
  }

  EntityStorage::Chunk::~Chunk()
  {
    reset();
  }

  auto EntityStorage::Chunk::hasFreePlace() const -> bool
  {
    return usedBlocks < capacity;
  }

  EntityStorage::EntityStorage(const size_t entity_size)
  {
    m_MaxEntitiesCountInChunk = MAX_CHUNK_SIZE / entity_size;
    m_EntitySize = entity_size;
    m_Chunks.emplace_back(m_EntitySize, m_MaxEntitiesCountInChunk);
  }

  EntityStorage::PlaceRequest EntityStorage::allocateEntityPlace()
  {
    size_t chunksCount = m_Chunks.size();
    for (size_t i = 0; i < chunksCount; ++i)
    {
      Chunk& chunk = m_Chunks[i];
      if (chunk.hasFreePlace())
      {
        chunk_eid_t chEid = (chunk_eid_t)chunk.usedBlocks;
        chunk.usedBlocks++;
        return {
          .chunkId = (chunk_id_t)i,
          .chunkEid = chEid
        };
      }
    }

    chunk_id_t chunkId = (chunk_id_t)chunksCount;
    m_Chunks.emplace_back(m_EntitySize, m_MaxEntitiesCountInChunk);
    return {
      .chunkId = chunkId,
      .chunkEid = (chunk_eid_t)m_Chunks.back().usedBlocks++
    };
  }

  void EntityStorage::placeEntity(const components_count_t count,
                                  const std::byte* __restrict__ comps_data,
                                  const component_offset_t* __restrict__ offsets,
                                  const component_size_t* __restrict__ sizes,
                                  const EntityId eid,
                                  const chunk_id_t chunk_id,
                                  const chunk_eid_t chunk_eid)
  {
    Chunk& chunk = m_Chunks[chunk_id];
    chunk.blocksOwners[chunk_eid] = eid;

    std::byte* __restrict__ data = chunk.data + m_EntitySize * chunk_eid;
    for (components_count_t i{0}; i < count; ++i)
    {
      const void* __restrict__ compSrc = comps_data + offsets[i];
      void* __restrict__ compDst = data + offsets[i];
      std::memcpy(compDst, compSrc, sizes[i]);
    }
  }

  auto EntityStorage::removeEntity(const chunk_id_t chunk_id,
                                   const chunk_eid_t chunk_eid) -> RemovementResult
  {
    RemovementResult res;

    Chunk& chunk = m_Chunks[chunk_id];
    const chunk_eid_t lastEid = chunk_eid_t{(uint16_t)(chunk.usedBlocks-1)};
      
    if (chunk_eid != lastEid)
    {
      res.movedEntity = chunk.blocksOwners[lastEid];
      res.newEntityChunkEid = chunk_eid;

      std::byte* lastEntityData = chunk.data + m_EntitySize * lastEid;
      std::byte* removingEntityData = chunk.data + m_EntitySize * chunk_eid;
      std::memcpy(removingEntityData, lastEntityData, m_EntitySize);
    }

    std::memset(chunk.data + m_EntitySize*lastEid, 0, m_EntitySize);
    chunk.blocksOwners[lastEid] = EntityId{};
    --chunk.usedBlocks;

    return res;
  }

  auto EntityStorage::getChunks() const -> const eastl::vector<Chunk>&
  {
    return m_Chunks;
  }
}
