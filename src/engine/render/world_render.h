#pragma once

#include "frame_gc.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/render/debug_text_queue.hpp>
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
  constexpr size_t FRAMES_COUNT = 3;

  class FontRender;

  class WorldRender
  {
    public:
      void init();
      void render(const CameraData& camera);

      inline float getWndAspect() const
      {
        return m_Aspect;
      }

      inline void pushDbgText(string&& text, const float4 color)
      {
        m_DbgTextQueue.push(std::move(text), color);
      }

    private:
      struct FrameData
      {
        std::unique_ptr<gapi::CmdEncoder> cmdEncoder;
        CameraData camera;
        fg::FrameGraph* fg;
      };
      FrameData m_FrameData;

      void beforeRender(const CameraData& camera);
      void renderWorld();
      void renderOpaque(gapi::CmdEncoder& encoder);
      void renderScene(gapi::CmdEncoder& encoder);
      void resolveGbuffer(gapi::CmdEncoder& encoder,
                          const gapi::TextureHandle albedo, const gapi::TextureHandle normal,
                          const gapi::TextureHandle worldPos, const gapi::TextureHandle metalRoughness);
      void renderDbgText(gapi::CmdEncoder& encoder);
    private:
      size_t m_FrameId = 0;
      fg::FrameGraph m_FrameGraphs[FRAMES_COUNT];

      gapi::SamplerHandler m_ModelSampler;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandle m_RtDepth;

      std::unique_ptr<FontRender> m_FontRender;
      dbg::TextDbgQueue<10, 128> m_DbgTextQueue;
  };

  extern WorldRender world_render;
}