#pragma once

#include <engine/render/gapi/resources.h>

namespace Engine::Render
{
  class WorldRender
  {
    public:
      void Init();
      void Render();

    private:
      gapi::BufferHandler m_TestBuffer;
  };

  extern WorldRender world_render;
}