#pragma once

#include <engine/types.h>
#include <engine/gapi/resource_wrappers.h>

#include <EASTL/vector.h>

namespace Engine::Render
{
  class TerrainRender
  {
    public:
      TerrainRender() = default;
      TerrainRender(const TerrainRender&);
      TerrainRender(const string& vterrain_name,
                    const int patch_side_bits,
                    const int world_size_meters,
                    const float2 vterrain_heightmap_min_max_border,
                    const float vterrain_heightmap_max_height);

      TerrainRender(TerrainRender&&) = default;
      TerrainRender& operator=(TerrainRender&&) = default;

      void setView(const float3 view_pos);
      void updateGpuData(gapi::CmdEncoder&);
      void render(gapi::CmdEncoder&);

    private:
      string m_TerrainAlbedo;
      string m_TerrainHeightmap;

      int m_PatchSideBits;
      int m_PatchSideSize;
      int m_WorldSizeMeters;
      int m_MaxLods;
      float2 m_MinMaxHeightBorder;
      float m_MaxHeight;

      struct Patch
      {
        float4 params; //worldXZ_SideSize_edgeType
      };
      eastl::vector<Patch> m_CulledPatches;
      gapi::BufferWrapper m_PatchesBufInfo;
  };
}
