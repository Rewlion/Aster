#pragma once

#include "vtile.h"

#include <engine/types.h>
#include <engine/gapi/resource_wrappers.h>

#include <EASTL/array.h>
#include <EASTL/hash_map.h>
#include <EASTL/span.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>

#include <EASTL/bonus/ring_buffer.h>
#include <function2/function2.hpp>

namespace Engine::Render 
{
  using TileRenderCB = fu2::function<void(gapi::CmdEncoder&) const>;

  class VirtualTexture
  {
    constexpr static size_t TILE_SIZE = 256;
    constexpr static size_t PHYSICAL_TILES = 40;
    constexpr static size_t MAX_INDIRECTION_SIZE = 2048;

    public:
      void init(gapi::CmdEncoder&, const float side_size_meter, const float texel_size_meter);
      void update(gapi::CmdEncoder&, eastl::span<VTile> feedback, const TileRenderCB&);
      void invalidate(gapi::CmdEncoder&);
      auto getSideSize() const -> size_t { return m_VTexSideSize; }
      auto getMaxMip() const -> size_t { return m_MaxMip; }
      auto getPhysTilesCache() const -> gapi::TextureHandle { return m_PhysTilesCache; }

      static
      auto getTileSize() -> size_t { return TILE_SIZE; }

      auto getPhysTex() const -> gapi::TextureHandle { return m_PhysTilesCache; }
      auto getPhysTexSize() const -> uint { return m_PhysTexSideSide; }
      auto getIndirectionTex() const -> gapi::TextureHandle { return m_PageTable.getIndirectionTex(); }
      auto getIndirectionTexSize() const -> uint { return m_PageTable.getIndirectionTexSize(); }

    private:
      class VTileLRU
      {
        public:
          struct Entry
          {
            VTile vtile;
            VTile physTile;
          };

          auto set(const VTile) -> Entry;
          void init(size_t phys_tiles_per_dim);
          void updateVTileEntry(const VTile);
          void invalidate();

        private:
          size_t m_PhysTilesPerDim = 0;

          using EntriesRingBuffer = eastl::ring_buffer<Entry>;
          EntriesRingBuffer m_Entries;
          eastl::hash_map<VTile, EntriesRingBuffer::iterator> m_VTileHashMap;
      };

      enum class VTileStatus : uint8_t
      {
        NotCached = 0, PageFault = 1, InCache = 2
      };

      class PageTable
      {
        struct IndirectionMip
        {
          struct IndirectionUpdateRequest { VTile indirectionTile; VTile physTile; };

          size_t tilesPerDim = 0;
          eastl::vector<VTileStatus> statuses;
          eastl::vector<VTile> pageFaultsBuffer;
          eastl::vector_map<VTile, VTile> indirectionUpdateBuffer;

          void invalidate();
        };
        public:
          using PageFaultHandler = fu2::function<void(size_t mip, VTile vtile, VTile ptile)>;

          void init(gapi::CmdEncoder&, const size_t side_size, const size_t mips);
          void processFeedback(eastl::span<VTile> feedback, VTileLRU& vtile_lru);
          auto hasPageFaults() const -> bool;
          void processPageFaults(PageFaultHandler, VTileLRU&);
          void processIndirectionClears(gapi::CmdEncoder&);
          void invalidate(gapi::CmdEncoder&);
          auto getIndirectionTex() const -> gapi::TextureHandle { return m_IndirectionTex; }
          auto getIndirectionTexSize() const -> uint { return m_TilesPerDim; }
        private:
          size_t m_TilesPerDim;
          gapi::TextureWrapper m_IndirectionTex;
          eastl::vector<IndirectionMip> m_IndirectionMips;
      };

    private:
      size_t m_VTexSize = 0;
      size_t m_MaxMip = 0;
      size_t m_VTexSideSize = 0;
      size_t m_PhysSideSizeMeters = 0;
      size_t m_PhysTexSideSide = 0;
      float m_TexelPerMeter = 0.0;

      VTileLRU m_VTileLRU;
      PageTable m_PageTable;
      gapi::TextureWrapper m_PhysTilesCache;
  };
}
