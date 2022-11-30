#include "debug_text.h"

#ifdef DEBUG
#include <engine/render/world_render.h>
#endif

namespace Engine::dbg
{
  #if DEBUG
    void vis_logerr(string&& text)
    {
      Render::world_render.pushDbgText(std::move(text), float4(1, 0, 0, 1));
    }

    void vis_loginfo(string&& text)
    {
      Render::world_render.pushDbgText(std::move(text), float4(0.9, 0.9, 0.9, 1));
    }
  #else
    void vis_logerr(string&& text)
    {
    }

    void vis_loginfo(string&& text)
    {
    }
  #endif
}
