#include "terrain_render.h"

#include <engine/assets/assets_manager.h>
#include <engine/ecs/type_meta.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>

#include <engine/shaders/shaders/terrain/edge_factor.hlsli>


//todo: cull by frustum

namespace Engine::Render
{
  TerrainRender::TerrainRender(const TerrainRender&)
  {
    ASSERT(!"Unexpected ecs call");
  }

  TerrainRender::TerrainRender(const string& vterrain_name,
                               const int patch_side_bits,
                               const int world_size_meters,
                               const float2 vterrain_heightmap_min_max_border,
                               const float vterrain_heightmap_max_height)
    : m_TerrainAlbedo(vterrain_name + ".albedo")
    , m_TerrainHeightmap(vterrain_name + ".heightmap")
    , m_PatchSideBits(patch_side_bits)
    , m_PatchSideSize(1<<patch_side_bits)
    , m_WorldSizeMeters(world_size_meters)
    , m_MaxLods(5)
    , m_MinMaxHeightBorder(vterrain_heightmap_min_max_border)
    , m_MaxHeight(vterrain_heightmap_max_height)
  {
  }

  void TerrainRender::setView(const float3 view_pos)
  {
    const int patchLodLimit = 3;
    int lod0Radius = 1 << (m_PatchSideBits-1);

    const auto alignAtBorder = [&](float v)
    {
      int alignSize = (lod0Radius << patchLodLimit)/16;
      return v - fmod(v, alignSize);
    };
    float2 viewOriginXZ{alignAtBorder(view_pos.x), alignAtBorder(view_pos.z)};

    int2 worldLeftTop = int2(-m_WorldSizeMeters/2,m_WorldSizeMeters/2);
    int worldRadius = m_WorldSizeMeters/2;

    struct MinMax {int2 min; int2 max;};
    const auto getMinMax = [](int2 left_top, int size)
    {
      return MinMax{
        .min = {left_top.x,      left_top.y-size},
        .max = {left_top.x+size, left_top.y}
      };
    };

    const auto intersectsAABB = [getMinMax](int2 left_top_a, int size_a, int2 left_top_b, int size_b)
    {
      auto a = getMinMax(left_top_a, size_a);
      auto b = getMinMax(left_top_b, size_b);

      auto [minA, maxA] = getMinMax(left_top_a, size_a);
      auto [minB, maxB] = getMinMax(left_top_b, size_b);

      return (minA.x < maxB.x && maxA.x > minB.x) &&
             (minA.y < maxB.y && maxA.y > minB.y);
    };

    //A is fully inside B
    const auto fullyInsideAABB = [getMinMax](int2 left_top_a, int size_a, int2 left_top_b, int size_b)
    {
      auto [minA, maxA] = getMinMax(left_top_a, size_a);
      auto [minB, maxB] = getMinMax(left_top_b, size_b);

      return (minA.x >= minB.x && maxA.x <= maxB.x) &&
             (minA.y >= minB.y && maxA.y <= maxB.y);
    };

    const auto isValidLodPatch = [&](int2 left_top, int patch_side_size, int prevLodRadius)
    {
      const int prevLodSize = prevLodRadius * 2;
      const int2 prevLodLeftTop = int2(viewOriginXZ) + int2(-prevLodRadius, prevLodRadius);

      const bool insideValidLodZone = prevLodRadius <= 0 ?
        true :
        !intersectsAABB(left_top, patch_side_size, prevLodLeftTop, prevLodSize);

      const bool insideWorldMap = intersectsAABB(left_top, patch_side_size,
                                                 worldLeftTop, worldRadius*2);

      return insideValidLodZone && insideWorldMap;
    };

    const auto calcEdgeFactor = [](int x, int y, int N)
    {
      uint factor = 0;

      if (x == 0)
        factor |= 1 << TS_EDGE_FACTOR_LEFT;
      else if (x == (N-1))
        factor |= 1 << TS_EDGE_FACTOR_RIGHT;

      if (y == 0)
        factor |= 1 << TS_EDGE_FACTOR_BOT;
      else if (y == (N-1))
        factor |= 1 << TS_EDGE_FACTOR_TOP;

      return factor;
    };

    const auto cullLodPatches = [&](int patch_side_size, int lod_r, int prev_lod_r, bool has_edge_factors = false)
    {
      const size_t N = 2 * lod_r / patch_side_size;
      int2 lt = int2(viewOriginXZ) + int2(-lod_r, lod_r);

      for (int x = 0; x < N; ++x)
        for (int y = 0; y < N; ++y)
      {
        uint edgeFactor = has_edge_factors ? calcEdgeFactor(x,y,N) : 0;
        int2 patchLT = lt + int2{x, -y} * patch_side_size;
        if (isValidLodPatch(patchLT, patch_side_size, prev_lod_r))
          m_CulledPatches.push_back(Patch{.params = float4{patchLT, patch_side_size, (float)edgeFactor} });
      }
    };

    m_CulledPatches.clear();
    cullLodPatches(1, lod0Radius, 0);

    int2 lt{0,0};
    int r = 0;
    int basePatchSize = lod0Radius;
    int lod = 0;
    int prevLodSize = lod0Radius;

    while (!fullyInsideAABB(worldLeftTop, worldRadius*2, lt, r*2))
    {
      int patchSize = basePatchSize << (std::min(lod, patchLodLimit));
      int lodRadius = (basePatchSize << lod) * 2;

      const bool hasEdgeFactors = lod < patchLodLimit;
      cullLodPatches(patchSize, lodRadius, prevLodSize, hasEdgeFactors);

      prevLodSize = lodRadius;
      lod += 1;

      r = lodRadius;
      lt = int2(viewOriginXZ) + int2(-r, r);
    }
  }

  void TerrainRender::updateGpuData(gapi::CmdEncoder& encoder)
  {
    const size_t patchesBufSize = m_CulledPatches.size() * sizeof(m_CulledPatches[0]);
    gapi::BufferHandler patchesBuf = gapi::allocate_buffer(patchesBufSize, gapi::BF_GpuVisible | gapi::BF_BindUAV);
    m_PatchesBufInfo = gapi::BufferWrapper{patchesBuf};

    encoder.writeBuffer(m_PatchesBufInfo, m_CulledPatches.data(), 0, patchesBufSize);
    encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_SRV);
    
    tfx::set_extern("terrainPatches", (gapi::BufferHandler)m_PatchesBufInfo);
  }

  void TerrainRender::render(gapi::CmdEncoder& encoder)
  {
    Engine::TextureAsset asset;
    Engine::assets_manager.getTexture(m_TerrainHeightmap, asset);
    tfx::set_channel("terrainHeightmap", asset.texture);
    Engine::assets_manager.getTexture(m_TerrainAlbedo, asset);
    tfx::set_channel("terrainAlbedo", asset.texture);

    tfx::set_channel("worldMapSize", (float)m_WorldSizeMeters);
    tfx::set_channel("terrainMinMaxHeightBorder", m_MinMaxHeightBorder);
    tfx::set_channel("terrainMaxHeight", m_MaxHeight);

    tfx::activate_technique("Terrain", encoder);
    encoder.updateResources();
    encoder.draw(m_CulledPatches.size(), 1, 0, 0);
  }

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT(TerrainRender);
}
