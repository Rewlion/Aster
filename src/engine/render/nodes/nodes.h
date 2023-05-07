#pragma once

#include <engine/render/frame_graph/node_id.h>

namespace Engine::Render
{
  fg::NodeHandle mk_backbuffer_acquiring_node();
  fg::NodeHandle mk_frame_preparing_node();
  fg::NodeHandle mk_gbuffer_main_pass_node();
  fg::NodeHandle mk_gbuffer_resolve_node();
  fg::NodeHandle mk_ui_node();
  fg::NodeHandle mk_dbg_text_node();
  fg::NodeHandle mk_present_node();
}
