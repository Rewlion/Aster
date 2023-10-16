#pragma once

#include <engine/types.h>
#include <engine/gapi/resource_wrappers.h>

#include <EASTL/vector.h>

namespace gapi
{
  class CmdEncoder;
}

namespace Engine::dbg
{
  class PolyRenderer
  {
    struct Vertex
    {
      float3 pos; 
      float4 color;
    };
    struct PolyVertices
    {
      Vertex a,b,c;
    };

    public:
      PolyRenderer() = default;
      PolyRenderer(const PolyRenderer& ecs_tmpl) {}

      void addPoly(const float3& a, const float3& b, const float3& c,
                   const float4& color, float lifetime_sec);
      void tick();
      void beforeRender(gapi::CmdEncoder& encoder);
      void render(gapi::CmdEncoder& encoder);

    private:
      bool m_Dirty = false;
      eastl::vector<float> m_PolyDeathTimeSec;
      eastl::vector<PolyVertices> m_StagingVertices;
      gapi::BufferWrapper m_VertexBuffer;
  };
}