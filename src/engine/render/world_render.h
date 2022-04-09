#pragma once

#include <engine/gapi/resources.h>

#include <engine/gapi/cmd_encoder.h>

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
      gapi::BufferHandler m_TestConstBuffer;

      gapi::CmdEncoder m_CmdEncoder;
  };

  extern WorldRender world_render;
}