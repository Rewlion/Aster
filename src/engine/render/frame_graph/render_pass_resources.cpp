#include "render_pass_resources.h"
#include <engine/gapi/resources.h>
#include <engine/render/frame_graph/frame_graph.h>

namespace fg
{
  RenderPassResources::RenderPassResources(FrameGraph& fg)
    : m_Fg(fg)
  {
  }

  TextureResourceView RenderPassResources::getTexture(const std::string_view name) const
  {
    return m_Fg.getTexture(name);
  }

  gapi::BufferHandler RenderPassResources::getBuffer(const std::string_view name) const
  {
    return m_Fg.getBuffer(name);
  }
}
