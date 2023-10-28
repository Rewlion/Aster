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
  };

  extern WorldRender world_render;
}