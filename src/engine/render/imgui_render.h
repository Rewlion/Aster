#pragma once

#include "frame_gc.h"

#include <engine/gapi/resource_wrappers.h>

struct ImDrawData;

namespace Engine::Render
{
  class ImGuiRender
  {
    public:
      void init();
      ~ImGuiRender();

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
