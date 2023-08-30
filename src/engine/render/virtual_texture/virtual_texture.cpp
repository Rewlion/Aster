#include "virtual_texture.h"

#include <engine/assert.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>

#include <EASTL/sort.h>

#include <ranges>
#include <bit>

namespace Engine::Render
{
  void VirtualTexture::init(gapi::CmdEncoder& encoder, const float side_size_meter, const float texel_size_meter)
  {
    float vTexF = side_size_meter / texel_size_meter;
    ASSERT_FMT(std::fmod(vTexF, (float)TILE_SIZE) == 0, 
      "VTEX: final Virtual Texture size must be multiple of tile size({}). Current Size: ({})", TILE_SIZE, side_size_meter);

    m_VTexSize = (size_t)vTexF;
    m_VTexSideSize = (size_t)vTexF;
    m_PhysSideSizeMeters = (size_t)side_size_meter;
    m_PhysTexSideSide = TILE_SIZE * PHYSICAL_TILES;
    m_TexelPerMeter = texel_size_meter;

    const size_t pageTableSize = m_VTexSize / TILE_SIZE;
    ASSERT_FMT(pageTableSize <= MAX_INDIRECTION_SIZE,
      "VTEX: too big indirection texture size ({}), max: ({})", pageTableSize, MAX_INDIRECTION_SIZE);

    const size_t mipsCount = std::bit_width(pageTableSize);
    m_MaxMip = mipsCount-1;
    ASSERT(mipsCount > 1);

    m_PageTable.init(encoder, pageTableSize, mipsCount);
    m_VTileLRU.init(PHYSICAL_TILES);

    m_PhysTilesCache = {
      gapi::allocate_texture({
        .format = gapi::TextureFormat::R8G8B8A8_UNORM,
        .extent = {m_PhysTexSideSide, m_PhysTexSideSide, 1},
        .mipLevels = 1,
        .usage = gapi::TEX_USAGE_RT | gapi::TEX_USAGE_SRV,
      })
    };
    encoder.clearColorTexture(m_PhysTilesCache, gapi::TextureState::Undefined, gapi::TextureState::ShaderRead, {(uint32_t)~0}, {});
  }

  void VirtualTexture::update(gapi::CmdEncoder& encoder, eastl::span<VTile> feedback, const TileRenderCB& render_cb)
  {
    m_PageTable.processFeedback(feedback, m_VTileLRU);
     if (!m_PageTable.hasPageFaults())
      return;

    encoder.transitTextureState(m_PhysTilesCache, gapi::TextureState::ShaderRead, gapi::TextureState::RenderTarget);
    encoder.beginRenderpass({gapi::RenderPassAttachment{m_PhysTilesCache}}, {});

    const auto pageFaultHandler = [&](size_t mip, VTile vtile, VTile ptile)
    {
      gapi::Scissor sc;
      sc.offset = int2{ptile.t.x, ptile.t.y} * (int)TILE_SIZE;
      sc.size = {TILE_SIZE, TILE_SIZE};
      
      gapi::Viewport vp {
        .pos = { sc.offset },
        .size = sc.size
      };
      encoder.setViewport(vp);
      encoder.setScissor(sc);

      tfx::set_extern("vtexTileSize", (float)TILE_SIZE);
      tfx::set_extern("packedVTile", (uint)vtile.r);
      tfx::set_extern("packedPTile", (uint)ptile.r);
      tfx::set_extern("vtexMip", (uint)mip);
      tfx::set_extern("vtexTexelSize", m_TexelPerMeter);
      tfx::activate_scope("VirtualTextureScope", encoder);

      render_cb(encoder);
    };

    m_PageTable.processPageFaults(pageFaultHandler, m_VTileLRU);

    encoder.endRenderpass();
    encoder.transitTextureState(m_PhysTilesCache, gapi::TextureState::RenderTarget, gapi::TextureState::ShaderRead);

    m_PageTable.processIndirectionClears(encoder);
  }

  void VirtualTexture::invalidate(gapi::CmdEncoder& encoder)
  {
    encoder.clearColorTexture(m_PhysTilesCache, gapi::TextureState::ShaderRead, gapi::TextureState::ShaderRead, {(uint32_t)~0}, {});

    m_PageTable.invalidate(encoder);
    m_VTileLRU.invalidate();
  }
}
