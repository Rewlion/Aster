#pragma once

#include "core_types.h"
#include "eid.h"

#include <EASTL/vector.h>
#include <EASTL/functional.h>

namespace ecs
{
  class EntityStorage
  {
    struct Chunk
    {
      Chunk(const size_t entity_size, const size_t capacity);
      ~Chunk();

      void reset();
      auto hasFreePlace() const -> bool;

      std::byte* data = nullptr;
      EntityId* blocksOwners = nullptr;
      uint16_t capacity = 0;
      uint16_t usedBlocks = 0;
    };

    public:
      EntityStorage(const size_t entity_size);

      struct PlaceRequest
      {
        chunk_id_t chunkId;
        chunk_eid_t chunkEid;
      };
      auto allocateEntityPlace() -> PlaceRequest;

      void placeEntity(const components_count_t,
                       const std::byte* __restrict__ comps_data,
                       const component_offset_t* __restrict__,
                       const component_size_t* __restrict__,
                       const EntityId,
                       const chunk_id_t,
                       const chunk_eid_t);

      struct RemovementResult
      {
        EntityId movedEntity;
        chunk_eid_t newEntityChunkEid = INVALID_CHUNK_EID;
      };
      auto removeEntity(const chunk_id_t,
                        const chunk_eid_t) -> RemovementResult;

      auto getChunks() const -> const eastl::vector<Chunk>&;

    private:
      eastl::vector<Chunk> m_Chunks;
      size_t m_MaxEntitiesCountInChunk;
      size_t m_EntitySize;
  };
}
