#pragma once

#include <engine/gapi/resource_wrappers.h>
#include <engine/render/frame_gc.h>

struct ImDrawData;

namespace Engine::Render
{
  class ImGuiRender
  {
    public:
      ImGuiRender() = default;
      ImGuiRender(const ImGuiRender& ecs_tmpl) {};
      const ImGuiRender& operator=(const ImGuiRender& ecs_tmpl) { return *this; }
      ~ImGuiRender();

      void init();

      void beforeRender(gapi::CmdEncoder&);
      void render(gapi::CmdEncoder&);

    private:
      struct DrawData
      {
        const ImDrawData* imDrawData;
        gapi::BufferHandler vb;
        gapi::BufferHandler ib;
        size_t count;
      };

      ImGuiRender::DrawData GatherDrawData(gapi::CmdEncoder&, const ImDrawData*);

    private:
      gapi::TextureWrapper m_FontTexture;
      FrameGC m_FrameGC;
      DrawData m_DrawData;
  };
}
