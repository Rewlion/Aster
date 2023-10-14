#include "line_render.h"

#include <engine/ecs/type_meta.h>
#include <engine/gapi/cmd_encoder.h>
#include <engine/tfx/tfx.h>
#include <engine/time.h>

namespace Engine::dbg
{
  void LineRenderer::addLine(const float3& start, const float3& end,
                             const float3& color, float lifetime_sec)
  {
    m_LineDeathTimeSec.emplace_back(Time::get_sec_since_start() + lifetime_sec);
    m_StagingVertices.push_back({start, color});
    m_StagingVertices.push_back({end, color});
    m_Dirty = true;
  }

  void LineRenderer::tick()
  {
    const float curTimeSec = Time::get_sec_since_start();
    size_t N = m_LineDeathTimeSec.size();
    for (size_t i = 0; i < N; ++i)
    {
      const float deathTimeSec = m_LineDeathTimeSec[i];
      if (deathTimeSec <= curTimeSec)
      {
        std::swap(m_StagingVertices[2*N-1-1], m_StagingVertices[i*2]);
        std::swap(m_StagingVertices[2*N-1], m_StagingVertices[i*2+1]);
        m_StagingVertices.resize(m_StagingVertices.size()-2);

        std::swap(m_LineDeathTimeSec[N-1], m_LineDeathTimeSec[i]);
        m_LineDeathTimeSec.pop_back();

        --N;
        m_Dirty = true;
      }
    }
  }

  void LineRenderer::beforeRender(gapi::CmdEncoder& encoder)
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

  void LineRenderer::render(gapi::CmdEncoder& encoder)
  {
    if (m_VertexBuffer != gapi::BufferHandler::Invalid)
    {
      tfx::activate_technique("DebugLine", encoder);
      encoder.bindVertexBuffer(m_VertexBuffer);
      encoder.updateResources();
      encoder.draw(m_StagingVertices.size(), 1, 0, 0);
    }
  }

  DECLARE_NON_TRIVIAL_ECS_OBJECT_COMPONENT_WITH_NAME(LineRenderer, "DbgLineRenderer");
}
