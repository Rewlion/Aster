#pragma once

#include "frame_gc.h"

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
      void updateFrameUniforms();
      void renderWorld();
      void renderOpaque();
      void renderScene(const RenderPassType rpType);
    private:
      FrameGC m_FrameGC;

      gapi::SamplerHandler m_ModelSampler;
      gapi::BufferHandler m_FrameUniforms;
      gapi::BufferHandler m_StaticMeshUniforms;

      gapi::CmdEncoder m_CmdEncoder;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandler m_RtDepth;
  };

  extern WorldRender world_render;
}