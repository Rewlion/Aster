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
  class LineRenderer
  {
    struct Vertex
    {
      float3 pos; 
      float3 color;
    };

    public:
      LineRenderer() = default;
      LineRenderer(const LineRenderer& ecs_tmpl) {}

      void addLine(const float3& start, const float3& end,
                   const float3& color, float lifetime_sec);
      void tick();
      void beforeRender(gapi::CmdEncoder& encoder);
      void render(gapi::CmdEncoder& encoder);

    private:
      bool m_Dirty = false;
      eastl::vector<float> m_LineDeathTimeSec;
      eastl::vector<Vertex> m_StagingVertices;
      gapi::BufferWrapper m_VertexBuffer;
  };
}
