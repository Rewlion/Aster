#pragma once

#include <engine/render/virtual_texture/virtual_texture.h>

#include <engine/types.h>
#include <engine/gapi/resource_wrappers.h>

#include <EASTL/vector.h>

namespace ed
{
  class Scope;
}

class VTerrainComponent;

namespace Engine::Render
{
  class TerrainRender
  {
    public:
      TerrainRender() = default;
      TerrainRender(const TerrainRender&);
      TerrainRender(const VTerrainComponent&);
      TerrainRender(const ed::Scope*) {}

      TerrainRender(TerrainRender&&) = default;
      TerrainRender& operator=(TerrainRender&&) = default;

      void setView(const VTerrainComponent& vt,const float3 view_pos);
      void updateGpuData(gapi::CmdEncoder&);
      void render(const VTerrainComponent& vt, gapi::CmdEncoder&, const gapi::TextureHandle feedback_buf, const int2 feedback_size);
      void updateVTex(const VTerrainComponent& vt, gapi::CmdEncoder&, const gapi::TextureHandle feedback_buf, const int2 feedback_size);

      auto getVTexTilesStorage() -> gapi::TextureHandle { return m_VirtualTexture.getTilesStorage(); }

    private:
      auto copyFeedbackFromGpu(gapi::CmdEncoder& encoder,
                               const gapi::TextureHandle feedback_buf,
                               const size_t feedback_size) -> gapi::BufferWrapper;
      auto analyzeFeedback(eastl::span<VTile> unprocessed_feedback) const -> eastl::vector<VTile>;

    private:
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
