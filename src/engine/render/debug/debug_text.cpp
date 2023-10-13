#include "debug_text.h"
#include "debug_text_queue.h"

#include <engine/render/ecs_utils.h>

namespace Engine::dbg
{
  void vis_logerr(string&& text)
  {
    if (auto* dbgQueue = Engine::Render::dbg::get_dbg_text_queue(); dbgQueue != nullptr)
      dbgQueue->push(std::move(text), float4(1, 0, 0, 1));
  }

  void vis_loginfo(string&& text)
  {
    if (auto* dbgQueue = Engine::Render::dbg::get_dbg_text_queue(); dbgQueue != nullptr)
      dbgQueue->push(std::move(text), float4(0.9, 0.9, 0.9, 1));
  }
}
