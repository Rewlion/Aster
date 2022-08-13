#pragma once

namespace fg
{
  class FrameGraph;

  class RenderPassResources
  {
    public:
      RenderPassResources(FrameGraph& fg);

    private:
      FrameGraph& m_Fg;
  };
}