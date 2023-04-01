#include "archetype.h"

namespace ecs
{
  auto Archetypes::addArchetype(entity_size_t entity_size,
                               eastl::vector_map<name_hash_t, component_id_t>&& name_to_comp_map,
                               eastl::vector<registered_component_id_t>&& comp_ids,                  
                               eastl::vector<component_offset_t>&& offsets,
                               eastl::vector<component_size_t>&& sizes,
                               eastl::vector<component_type_id_t>&& types) -> archetype_id_t
  {
    const archetype_id_t id = (archetype_id_t)m_Archetypes.size();
    EntityStorage storage{entity_size};

    components_set_hash_t compsIdsHash = hashCompIds(comp_ids);
    m_CompsHashToArchetypes.insert({compsIdsHash, id});

    m_Archetypes.emplace_back(
      std::move(entity_size),
      std::move(name_to_comp_map),
      std::move(storage),
      (components_count_t)types.size(),
      std::move(compsIdsHash),
      std::move(comp_ids),
      std::move(offsets),
      std::move(sizes),
      std::move(types)
    );

    return id;
  }

  auto Archetypes::getArchetypesCount() const -> archetype_id_t
  {
    return (archetype_id_t)m_Archetypes.size();
  }

  void Archetypes::forEachEntityInArchetype(const archetype_id_t arch_id,
                                           const eastl::function<void(ComponentsAccessor&)>& cb)
  {
    const auto& offsets = getComponentOffsets(arch_id);
    auto& chunks = getEntityStorage(arch_id).getChunks();
    const entity_size_t entitySize = getEntitySize(arch_id);
    const auto& nameToCompMap = getNameToComponentsMap(arch_id);

    const chunk_id_t chunksCount{chunks.size()};
    for (chunk_id_t chunkId{0}; chunkId < chunksCount; ++chunkId)
    {
      auto& chunk = chunks[chunkId];
      const chunk_eid_t entitiesCount{chunk.usedBlocks};
      for (chunk_eid_t chunkEid{0}; chunkEid < entitiesCount; ++chunkEid)
      {
        std::byte* entityData = chunk.data + entitySize * chunkEid;
        ComponentsAccessor accessor{entityData, offsets.data(), nameToCompMap};
        cb(accessor);
      }
    }
  }

  void Archetypes::accessEntityInArchetype(const archetype_id_t arch_id,
                                        const chunk_id_t chunk_id,
                                        const chunk_eid_t chunk_eid,
                                        const eastl::function<void(ComponentsAccessor&)>& cb)
  {
    const auto& offsets = getComponentOffsets(arch_id);
    auto& chunks = getEntityStorage(arch_id).getChunks();
    const entity_size_t entitySize = getEntitySize(arch_id);
    const auto& nameToCompMap = getNameToComponentsMap(arch_id);

    std::byte* entityData = chunks[chunk_id].data + entitySize * chunk_eid;
    ComponentsAccessor accessor{entityData, offsets.data(), nameToCompMap};
    cb(accessor);
  }

  auto Archetypes::findArchetypesWithComponents(const eastl::vector<registered_component_id_t>& comps)
    -> eastl::vector<archetype_id_t>
  {
    components_set_hash_t hash = hashCompIds(comps);
    const auto getDesiredArchs = [hash, this]() -> eastl::vector<archetype_id_t>
    {
      eastl::vector<archetype_id_t> res;
      const auto [lb, ub] = m_CompsHashToArchetypes.equal_range(hash);
      if (lb != m_CompsHashToArchetypes.end())
      {
        const size_t count = eastl::distance(lb, ub) + 1;
        res.reserve(count);
        for (auto it = lb; it != ub; ++it)
          res.push_back(it->second);
      }
      return res;
    };

    auto desiredArchs = getDesiredArchs();
    if (!desiredArchs.empty())
      return desiredArchs;

    const auto isDesiredArch = [this, &comps] (const archetype_id_t arch_id)
    {
      const eastl::vector<registered_component_id_t>& archComps = getComponentIds(arch_id);
      if (archComps.size() >= comps.size() )
      {
        for (auto required_id: comps)
        {
          const auto archId = eastl::find(archComps.begin(), archComps.end(), required_id);
          if (archId == archComps.end())
            return false;
        }
        return true;
      }

      return false;
    };

    const archetype_id_t archsCount = (archetype_id_t)m_Archetypes.size();
    for (archetype_id_t i{0}; i < archsCount; ++i)
      if (isDesiredArch(i))
        m_CompsHashToArchetypes.insert({hash, i});

    return getDesiredArchs();
  }

  auto Archetypes::hashCompIds(const eastl::vector<registered_component_id_t>& ids) const
    -> components_set_hash_t
  {
    uint32_t seed = 0;
    uint32_t hash = XXH32(ids.data(), ids.size() * sizeof(ids[0]), seed);
    return (components_set_hash_t) hash;
  }

  auto Archetypes::accessEntityComponents(const archetype_id_t arch_id,
                                          const chunk_id_t chunk_id,
                                          const chunk_eid_t chunk_eid) -> ComponentsAccessorById
  {
    const auto& offsets = getComponentOffsets(arch_id);
    const auto& types = getComponentTypes(arch_id);
    const auto compsCount = getComponentsCount(arch_id);
    auto& chunk = getEntityStorage(arch_id).getChunks()[chunk_id];
    const entity_size_t entitySize = getEntitySize(arch_id);
    std::byte* entityData = chunk.data + entitySize * chunk_eid;

    return ComponentsAccessorById{entityData, offsets.data(), types.data(), compsCount};
  }
}
