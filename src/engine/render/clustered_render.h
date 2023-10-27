#pragma once

#include <engine/gapi/resource_wrappers.h>

#include <engine/shaders/shaders/clustered_light/utils.hlsl>

namespace Engine::Render
{
  class ClusteredRenderer
  {
    public:
      ClusteredRenderer();
      ClusteredRenderer(const ClusteredRenderer& ecs_tmpl) {}

      void beforeRender(gapi::CmdEncoder&);
      void addPointLight(PointLight&&);
      void render(gapi::CmdEncoder&);

    private:
      void buildClusters(gapi::CmdEncoder& encoder);
      void cullLights(gapi::CmdEncoder& encoder);
      void packLights(gapi::CmdEncoder&);

    public:
      bool m_Dirty;
      eastl::vector<PointLight> m_PointLights;
      gapi::BufferWrapper m_LightsBuf;
      gapi::BufferWrapper m_IndirectionBuf;
      gapi::BufferWrapper m_IndirectionOffsetBuf;
      gapi::BufferWrapper m_ClustersInfoBuf;
      gapi::BufferWrapper m_FrustumClustersBuf;
  };
}
