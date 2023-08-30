#include "virtual_texture.h"

#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>

#include <EASTL/sort.h>

#include <ranges>
#include <bit>

namespace Engine::Render
{
  void VirtualTexture::PageTable::init(gapi::CmdEncoder& encoder, const size_t tiles_per_dim, const size_t mips)
  {
    m_TilesPerDim = tiles_per_dim;

    m_IndirectionTex = {
      gapi::allocate_texture({
        .format = gapi::TextureFormat::R32_UINT,
        .extent = {tiles_per_dim, tiles_per_dim, 1},
        .mipLevels = (uint32_t)mips,
        .usage = gapi::TEX_USAGE_UAV | gapi::TEX_USAGE_SRV,
      })
    };

    encoder.clearColorTexture(m_IndirectionTex, gapi::TextureState::Undefined, gapi::TextureState::ShaderRead,  {(uint32_t)~0}, {});
    
    m_IndirectionMips.resize(mips);
    for (int i = 0; i < mips; ++i)
    {
      size_t tilesPerDim = (tiles_per_dim >> i)+1;
      m_IndirectionMips[i].tilesPerDim = tilesPerDim;
      m_IndirectionMips[i].statuses.resize(tilesPerDim*tilesPerDim, VTileStatus::NotCached);
    }
  }

  void VirtualTexture::PageTable::processFeedback(eastl::span<VTile> feedback, VTileLRU& vtile_lru)
  {
    eastl::sort(feedback.begin(), feedback.end(),
      [](const VTile& l, const VTile& r)
      {
        return l.t.mip < r.t.mip;
      });

    auto mipRanges = feedback | std::views::chunk_by([](auto& ltile, auto& rtile){
      return ltile.t.mip == rtile.t.mip;
    });

    for (const auto& chunkedByMipFeedback : mipRanges)
    {
      const auto mipLevel = chunkedByMipFeedback[0].t.mip;
      ASSERT_FMT(mipLevel < m_IndirectionMips.size(), "vtile mip level({}) > max mip level ({})", mipLevel, m_IndirectionMips.size()-1);

      auto& indMip = m_IndirectionMips[mipLevel];
      for (const auto& vtile : chunkedByMipFeedback)
      {
        const size_t tileId = get_tile_id(vtile.r, indMip.tilesPerDim);
        auto& status = indMip.statuses[tileId];
        if (status == VTileStatus::InCache)
        {
          vtile_lru.updateVTileEntry(vtile);
        }
        else if (status == VTileStatus::NotCached)
        {
          status = VTileStatus::PageFault;
          indMip.pageFaultsBuffer.push_back(vtile);
        }
      }
    }
  }

  auto VirtualTexture::PageTable::hasPageFaults() const -> bool
  {
    for (const auto& indMip: m_IndirectionMips)
      if (!indMip.pageFaultsBuffer.empty())
        return true;

    return false;
  }

  void VirtualTexture::PageTable::processPageFaults(PageFaultHandler page_fault_handler, VTileLRU& vtile_lru)
  {
    size_t mips = m_IndirectionMips.size();
    for (int i = 0; i < mips; ++i)
    {
      auto& indMip = m_IndirectionMips[i];
      for (const auto pfTile: indMip.pageFaultsBuffer)
      {
        const VTileLRU::Entry lruEntry = vtile_lru.set(pfTile);
        if (lruEntry.vtile.r != pfTile.r)
        {
          const auto& oldVTile = lruEntry.vtile;
          auto& oldVTileIndMip = m_IndirectionMips[oldVTile.t.mip];
          const size_t oldVTileId = get_tile_id(oldVTile, oldVTileIndMip.tilesPerDim);
          
          auto& oldVTileStatus = oldVTileIndMip.statuses[oldVTileId];

          ASSERT_FMT(oldVTileStatus == VTileStatus::InCache, "old VTile[x:{} y:{} mip:{} id:{}] don't have InCache status. Current Status:{}",
            oldVTile.t.x, oldVTile.t.y, oldVTile.t.mip, oldVTileId, (int)oldVTileStatus);

          oldVTileStatus = VTileStatus::NotCached;

          oldVTileIndMip.indirectionUpdateBuffer.insert({lruEntry.vtile, VTile((uint32_t)~0)});
        }

        page_fault_handler(pfTile.t.mip, pfTile, lruEntry.physTile);
        const size_t pfTileId = get_tile_id(pfTile, indMip.tilesPerDim);

        auto& pfTileStatus = indMip.statuses[pfTileId];
        ASSERT_FMT(pfTileStatus == VTileStatus::PageFault, "VTile[x:{} y:{} mip:{}] don't have PageFault status. Current Status:{}",
          pfTile.t.x, pfTile.t.y, pfTile.t.mip, (int)pfTileStatus);
        pfTileStatus = VTileStatus::InCache;
        indMip.indirectionUpdateBuffer.insert({pfTile, lruEntry.physTile});
      }
      indMip.pageFaultsBuffer.clear();
    }
  }

  void VirtualTexture::PageTable::processIndirectionClears(gapi::CmdEncoder& encoder)
  {
    encoder.transitTextureState(m_IndirectionTex, gapi::TextureState::ShaderRead, gapi::TextureState::ShaderReadWrite);

    tfx::activate_technique("VTexIndirectionUpdate", encoder);

    eastl::vector<uint4> stagingBuf;
    size_t mips = m_IndirectionMips.size();
    for (size_t i = 0; i < mips; ++i)
    {
      auto& indMip = m_IndirectionMips[i];
      const auto& indBuf = indMip.indirectionUpdateBuffer;
      if (!indBuf.empty())
      {
        const size_t bufSize = indBuf.size() * sizeof(float4);
        stagingBuf.reserve(bufSize);
        for (const auto& [vtile, ptile] : indBuf)
          stagingBuf.push_back(uint4{vtile.r, ptile.r, 0, 0});

        gapi::BufferWrapper gpuBuf{gapi::allocate_buffer(bufSize, gapi::BF_GpuVisible | gapi::BF_BindUAV, "vtexUpdateBuffer")};
        encoder.writeBuffer(gpuBuf, stagingBuf.data(), 0, bufSize);

        encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_UAV_RW);
        tfx::set_extern("vtexUpdateBuffer", gpuBuf);
        tfx::set_extern("vtexUpdateCount", (uint)stagingBuf.size());
        tfx::set_extern("vtexIndirectionTex", tfx::Texture{m_IndirectionTex, i});
        tfx::activate_scope("VTexIndirectionUpdateScope", encoder);

        encoder.updateResources();
        const size_t nDispatchs = (stagingBuf.size() + 63) / 64;
        encoder.dispatch(nDispatchs, 1, 1);

        indMip.indirectionUpdateBuffer.clear();
        stagingBuf.clear();
      }
    }

    encoder.transitTextureState(m_IndirectionTex, gapi::TextureState::ShaderReadWrite, gapi::TextureState::ShaderRead);
  }

  void VirtualTexture::PageTable::invalidate(gapi::CmdEncoder& encoder)
  {
    for (auto& indMip: m_IndirectionMips)
      indMip.invalidate();

    encoder.clearColorTexture(m_IndirectionTex, gapi::TextureState::ShaderRead, gapi::TextureState::ShaderRead, {(uint32_t)~0}, {});
  }

  void VirtualTexture::PageTable::IndirectionMip::invalidate()
  {
    std::memset(statuses.data(), (size_t)VTileStatus::NotCached, statuses.size() * sizeof(statuses[0]));
    pageFaultsBuffer.clear();
    indirectionUpdateBuffer.clear();
  }
}