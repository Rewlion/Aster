#pragma once

#include "handles.h"

#include <engine/gapi/resources.h>

namespace fg
{
  class FrameGraph;

  class RenderPassResources
  {
    public:
      RenderPassResources(FrameGraph& fg);

      gapi::TextureHandler getTexture(const VirtualResourceHandle h) const;
      gapi::BufferHandler getBuffer(const VirtualResourceHandle h) const;

    private:
      FrameGraph& m_Fg;
  };
}