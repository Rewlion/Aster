#pragma once

#include <engine/render/virtual_texture/virtual_texture.h>

#include <engine/types.h>
#include <engine/gapi/resource_wrappers.h>

#include <EASTL/vector.h>

namespace ed
{
  class Scope;
}

namespace Engine::Render
{
  class TerrainRender
  {
    public:
      TerrainRender() = default;
      TerrainRender(const TerrainRender&);
      TerrainRender(const ed::Scope*) {}
      TerrainRender(const string& vterrain_name,
                    const int patch_side_bits,
                    const int world_size_meters,
                    const float2 vterrain_heightmap_min_max_border,
                    const float vterrain_heightmap_max_height,
                    const string& vterrain_detail,
                    const int vterrain_detailSize);

      TerrainRender(TerrainRender&&) = default;
      TerrainRender& operator=(TerrainRender&&) = default;

      void setView(const float3 view_pos);
      void updateGpuData(gapi::CmdEncoder&);
      void render(gapi::CmdEncoder&, const gapi::TextureHandle feedback_buf, const int2 feedback_size);
      void updateVTex(gapi::CmdEncoder&, const gapi::TextureHandle feedback_buf, const int2 feedback_size);

      auto getVTexTilesStorage() -> gapi::TextureHandle { return m_VirtualTexture.getTilesStorage(); }

    private:
      auto copyFeedbackFromGpu(gapi::CmdEncoder& encoder,
                               const gapi::TextureHandle feedback_buf,
                               const size_t feedback_size) -> gapi::BufferWrapper;
      auto analyzeFeedback(eastl::span<VTile> unprocessed_feedback) const -> eastl::vector<VTile>;

    private:
      string m_TerrainAlbedo;
      string m_TerrainHeightmap;
      string m_TerrainDetailAlbedo;

      int m_PatchSideBits;
      int m_PatchSideSize;
      int m_WorldSizeMeters;
      int m_MaxLods;
      float2 m_MinMaxHeightBorder;
      float m_MaxHeight;
      int m_DetailSize;

      struct Patch
      {
        float4 params; //worldXZ_SideSize_edgeType
      };
      eastl::vector<Patch> m_CulledPatches;
      gapi::BufferWrapper m_PatchesBufInfo;

      VirtualTexture m_VirtualTexture;
      gapi::BufferWrapper m_Feedback;
      std::unique_ptr<gapi::CmdPromise> m_FeedbackPromise;
  };
}
