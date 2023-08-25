#include "virtual_texture.h"

#include <engine/assert.h>

namespace Engine::Render
{
  void VirtualTexture::VTileLRU::init(size_t phys_tiles_per_dim)
  {
    m_PhysTilesPerDim = phys_tiles_per_dim;
    m_Entries.clear();
    m_VTileHashMap.clear();

    m_Entries.reserve(phys_tiles_per_dim*phys_tiles_per_dim);
    m_VTileHashMap.reserve(phys_tiles_per_dim);
  }

  auto VirtualTexture::VTileLRU::set(const VTile tile) -> Entry
  {
    if (m_Entries.full())
    {
      auto oldEntry = m_Entries.begin();
      m_VTileHashMap.erase(oldEntry->vtile);

      m_Entries.push_back();
      auto newEntry = --m_Entries.end();
      newEntry->vtile = tile;
      newEntry->physTile = oldEntry->physTile;

      m_VTileHashMap.insert({tile, newEntry});
      return *oldEntry;
    }

    const size_t usedPhysTiles = m_Entries.size();
    const uint2 newPhysTile =  get_tile_pos_from_id(usedPhysTiles, m_PhysTilesPerDim);

    m_Entries.push_back();
    auto newEntry = --m_Entries.end();
    newEntry->vtile = tile;
    newEntry->physTile = VTile(newPhysTile.x, newPhysTile.y, 0);
    m_VTileHashMap.insert({tile, newEntry});

    return *newEntry;
  }

  void VirtualTexture::VTileLRU::updateVTileEntry(const VTile tile)
  {
    auto it = m_VTileHashMap.find(tile);
    ASSERT_FMT(it != m_VTileHashMap.end(), "Unknown VTILE[x:{}, y:{}, mip:{}]", tile.t.x, tile.t.y, tile.t.mip);

    auto entry = it->second;
    if (entry != (--m_Entries.end()))
    {
      auto next = eastl::next(entry);
      ASSERT(next != m_Entries.end());
      std::swap(*entry, *next);
      m_VTileHashMap[next->vtile] = entry;
      m_VTileHashMap[tile] = next;
    }
  }

  void VirtualTexture::VTileLRU::invalidate()
  {
    m_Entries.clear();
    m_VTileHashMap.clear();
  }
}
