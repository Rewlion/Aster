#pragma once

#include "frame_gc.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/render/frame_graph/blackboard.hpp>
#include <engine/render/frame_graph/frame_graph.h>
#include <engine/work_cycle/camera.h>

#include <memory>

namespace fg
{
  class RenderPassResources;
}

namespace Engine::Render
{
  constexpr size_t FRAMES_COUNT = 2;

  namespace blackboard
  {
    struct Gbuffer;
    struct Frame;
  }

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
        fg::Blackboard blackboard;
        fg::FrameGraph* fg;
      };
      FrameData m_FrameData;

      void beforeRender(const CameraData& camera);
      void renderWorld();
      void renderOpaque(gapi::CmdEncoder& encoder);
      void renderScene(gapi::CmdEncoder& encoder);
      void resolveGbuffer(gapi::CmdEncoder& encoder,
                          const gapi::TextureHandler albedo, const gapi::TextureHandler normal,
                          const gapi::TextureHandler worldPos, const gapi::TextureHandler metalRoughness);
    private:
      size_t m_FrameId = 0;
      fg::FrameGraph m_FrameGraphs[FRAMES_COUNT];

      gapi::SamplerHandler m_ModelSampler;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandler m_RtDepth;
  };

  extern WorldRender world_render;
}