#pragma once

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/materials/materials.h>

namespace Engine::Render
{
  class WorldRender
  {
    public:
      void init();
      void render(const mat4& cameraVP);

      inline float getWndAspect() const
      {
        return m_Aspect;
      }

    private:
      struct FrameData
      {
        mat4 vp;
      };
      FrameData m_FrameData;

      void beforeRender(const mat4& cameraVP);
      void renderWorld();
      void renderOpaque();
      void renderScene(const RenderPassType rpType);
    private:
      gapi::SamplerHandler m_TestSampler;
      gapi::BufferHandler m_TestConstBuffer;

      gapi::CmdEncoder m_CmdEncoder;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandler m_RtDepth;
  };

  extern WorldRender world_render;
}