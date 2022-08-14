#include "render_pass_resources.h"
#include <engine/gapi/resources.h>
#include <engine/render/frame_graph/frame_graph.h>

namespace fg
{
  RenderPassResources::RenderPassResources(FrameGraph& fg)
    : m_Fg(fg)
  {
  }

  gapi::TextureHandler RenderPassResources::getTexture(const VirtualResourceHandle h) const
  {
    return m_Fg.getTexture(h);
  }

  gapi::BufferHandler RenderPassResources::getBuffer(const VirtualResourceHandle h) const
  {
    return m_Fg.getBuffer(h);
  }
}
