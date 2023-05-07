#pragma once

#include "consts.h"

#include <engine/gapi/cmd_encoder.h>
#include <engine/gapi/resources.h>
#include <engine/render/debug_text_queue.hpp>
#include <engine/work_cycle/camera.h>
#include <engine/render/frame_graph/node_id.h>

#include <memory>

namespace fg
{
  class RenderPassResources;
}

namespace Engine::Render
{
  class FontRender;
  class GuiRender;
  class ImGuiRender;


  class WorldRender
  {
    friend fg::NodeHandle mk_frame_preparing_node();
    friend fg::NodeHandle mk_ui_node();
    friend fg::NodeHandle mk_dbg_text_node();

    public:
      void init();
      void setupEcs() {};
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
      };
      FrameData m_FrameData;

      void createFrameGraph();

      void renderDbgText(gapi::CmdEncoder& encoder);
    private:
      size_t m_FrameId = 0;

      gapi::SamplerHandler m_ModelSampler;

      int2 m_WindowSize;
      float m_Aspect;

      gapi::TextureHandle m_RtDepth;

      std::unique_ptr<FontRender> m_FontRender;
      std::unique_ptr<GuiRender> m_GuiRender;
      std::unique_ptr<ImGuiRender> m_ImGuiRender;
      dbg::TextDbgQueue<10, 128> m_DbgTextQueue;

      eastl::vector<fg::NodeHandle> m_FgNodes;
  };

  extern WorldRender world_render;
}