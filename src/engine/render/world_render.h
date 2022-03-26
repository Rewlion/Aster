#pragma once

#include <engine/render/gapi/resources.h>

namespace Engine::Render
{
  class WorldRender
  {
    public:
      void init();
      void render();

    private:
      gapi::BufferHandler m_TestBuffer;
      gapi::BufferHandler m_TestIndexBuffer;
      gapi::SamplerHandler m_TestSampler;
  };

  extern WorldRender world_render;
}