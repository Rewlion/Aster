#pragma once

#include <engine/render/frame_graph/node_id.h>

namespace Engine::Render
{
  fg::node_id_t mk_backbuffer_acquiring_node();
  fg::node_id_t mk_frame_preparing_node();
  fg::node_id_t mk_gbuffer_main_pass_node();
  fg::node_id_t mk_gbuffer_resolve_node();
  fg::node_id_t mk_ui_node();
  fg::node_id_t mk_dbg_text_node();
  fg::node_id_t mk_present_node();
}
