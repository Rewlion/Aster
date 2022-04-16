#pragma once

#include <engine/gapi/resources.h>

#include <engine/gapi/cmd_encoder.h>

namespace Engine::Render
{
  class WorldRender
  {
    public:
      void init();
      void render(const mat4& cameraVP);

    private:
      struct FrameData
      {
        mat4 vp;
      };
      FrameData m_FrameData;

      void beforeRender(const mat4& cameraVP);
      void renderStaticSceneOpaque();

    private:
      gapi::SamplerHandler m_TestSampler;
      gapi::BufferHandler m_TestConstBuffer;

      gapi::CmdEncoder m_CmdEncoder;
  };

  extern WorldRender world_render;
}