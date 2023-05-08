#pragma once

#include "consts.h"

#include <engine/render/frame_graph/node_id.h>

#include <EASTL/vector.h>

namespace fg
{
  class RenderPassResources;
}

namespace Engine::Render
{

  class WorldRender
  {
    public:
      void init();
      void setupEcs();
      void render();

    private:
      void createFrameGraph();
    private:
      eastl::vector<fg::NodeHandle> m_FgNodes;
  };

  extern WorldRender world_render;
}