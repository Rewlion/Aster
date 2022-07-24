#pragma once

#include "frame_gc.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/work_cycle/camera.h>

#include <memory>

namespace Engine::Render
{
  class WorldRender
  {
    public:
      void init();
      void render(const CameraData& camera);

      inline float getWndAspect() const
      {
        return m_Aspect;
      }

    private:
      struct FrameData
      {
        std::unique_ptr<gapi::CmdEncoder> cmdEncoder;
        CameraData camera;
        gapi::TextureHandler depth;
      };
      FrameData m_FrameData;

      void beforeRender(const CameraData& camera);
      void renderWorld();
      void renderOpaque();
      void renderScene();
    private:
      FrameGC m_FrameGC;

      gapi::SamplerHandler m_ModelSampler;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandler m_RtDepth;
  };

  extern WorldRender world_render;
}