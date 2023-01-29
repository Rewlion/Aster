#pragma once

#include "eid.h"
#include "entity_cb.h"
#include "entity_initializer.h"
#include "query.h"
#include "types.h"

#include <engine/types.h>

namespace Engine::ECS
{
  struct ArchetypeComponentsDescription
  {
    eastl::vector<ComponentDescription> components;
    bool merge(const ArchetypeComponentsDescription&);
  };

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
      inline ArchetypeStorage(const size_t blockSize, ArchetypeComponentsDescription&& desc, ComponentMap&& comp_map)
        : m_BlockSize(blockSize)
        , m_Description(std::move(desc))
        , m_ComponentsMap(std::move(comp_map))
      {
      }

      ChunkInfo addEntity(EntityId, const CreationCb&);

      DestroidEntityInfo destroyEntity(const chunk_id chunkId, const block_id blockId);

      inline size_t getBlockSize() const
      {
        return m_BlockSize;
      }

      inline const ComponentMap& getComponentMap() const
      {
        return m_ComponentsMap;
      }

      inline const ArchetypeComponentsDescription& getDescription() const
      {
        return m_Description;
      }

      void forEachEntity(const ForEachCb& cb);

      bool hasComponents(const eastl::vector<ComponentDescription>&) const;
      bool hasComponent(const component_type_id, const component_name_id) const;

    private:
      chunk_id getFreeChunkId();
      ChunkInfo getFreeChunk(const EntityId eid);

    private:
      eastl::vector<Chunk> m_Chunks;
      size_t m_BlockSize;

      ArchetypeComponentsDescription m_Description;
      ComponentMap m_ComponentsMap;
  };
}