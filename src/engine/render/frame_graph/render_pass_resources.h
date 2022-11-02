#pragma once

#include "handles.h"
#include "resources.h"

namespace fg
{
  class FrameGraph;

  class RenderPassResources
  {
    public:
      RenderPassResources(FrameGraph& fg);

      TextureResourceView getTexture(const std::string_view name) const;
      gapi::BufferHandler getBuffer(const std::string_view name) const;

    private:
      FrameGraph& m_Fg;
  };
}