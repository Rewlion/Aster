#include "poly_render.h"

#include <engine/ecs/type_meta.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>
#include <engine/work_cycle/camera.h>

namespace Engine::dbg
{
  void PolyRenderer::addPoly(const float3& a, const float3& b, const float3& c,
                             const float4& color, float lifetime_sec)
  {
    auto& v = *reinterpret_cast<PolyVertices*>(m_StagingVertices.push_back_uninitialized());
    v.a = Vertex{a, color};
    v.b = Vertex{b, color};
    v.c = Vertex{c, color};

    m_PolyDeathTimeSec.emplace_back(Time::get_sec_since_start() + lifetime_sec);
    m_Dirty = true;
  }

  void PolyRenderer::tick()
  {
    const float curTimeSec = Time::get_sec_since_start();
    size_t N = m_PolyDeathTimeSec.size();
    for (size_t i = 0; i < N; ++i)
    {
      const float deathTimeSec = m_PolyDeathTimeSec[i];
      if (deathTimeSec <= curTimeSec)
      {
        std::swap(m_StagingVertices[N-1], m_StagingVertices[i]);
        m_StagingVertices.pop_back();

        std::swap(m_PolyDeathTimeSec[N-1], m_PolyDeathTimeSec[i]);
        m_PolyDeathTimeSec.pop_back();

        --N;
        m_Dirty = true;
      }
    }
  }

  void PolyRenderer::beforeRender(gapi::CmdEncoder& encoder)
  {
    if (m_Dirty)
    {
      if (!m_StagingVertices.empty())
      {
        const size_t stagingBufSize = sizeof(m_StagingVertices[0]) * m_StagingVertices.size();
        m_VertexBuffer.reset(gapi::allocate_buffer(stagingBufSize, gapi::BF_GpuVisible | gapi::BF_BindVertex));
        encoder.writeBuffer(m_VertexBuffer, m_StagingVertices.data(), 0, stagingBufSize);
        encoder.insertGlobalBufferBarrier(gapi::BufferState::BF_STATE_TRANSFER_DST, gapi::BufferState::BF_STATE_VERTEX);
        m_Dirty = false;
      }
      else
        m_VertexBuffer.reset();
    }
  }

  void PolyRenderer::render(gapi::CmdEncoder& encoder)
  {
    if (m_VertexBuffer != gapi::BufferHandler::Invalid)
    {
      tfx::activate_technique("DebugPoly", encoder);
      encoder.bindVertexBuffer(m_VertexBuffer);
      encoder.updateResources();
      encoder.draw(m_StagingVertices.size() * 3, 1, 0, 0);
    }
  }

  void PolyRenderer::combine(gapi::CmdEncoder& encoder)
  {
    if (m_VertexBuffer != gapi::BufferHandler::Invalid)
    {
      tfx::activate_technique("DebugPolyCombine", encoder);
      encoder.updateResources();
      encoder.draw(4, 1, 0, 0);
    }
  }

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(PolyRenderer, "DbgPolyRenderer");
}
