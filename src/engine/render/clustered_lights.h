#pragma once

#include <engine/gapi/resource_wrappers.h>

#include <engine/shaders/shaders/clustered_light/utils.hlsl>

namespace ed
{
  class Scope;
}

namespace Engine::Render
{
  class ClusteredLights
  {
    public:
      ClusteredLights();
      ClusteredLights(const ClusteredLights& ecs_tmpl) {}
      ClusteredLights(const ed::Scope*) {}

      void addPointLight(PointLight&&);

      void beforeRender(gapi::CmdEncoder&);
      void render(gapi::CmdEncoder&);

      void packLights(gapi::CmdEncoder& encoder);
      auto getLightsBuffer() -> gapi::BufferHandler
      {
        return m_LightsBuf;
      }

    private:
      void cullLights(gapi::CmdEncoder& encoder);

    public:
      bool m_Dirty;

      eastl::vector<PointLight> m_PointLights;
      gapi::BufferWrapper m_LightsBuf;
  };
}
