#pragma once

#include <engine/render/frame_graph/node_id.h>

namespace Engine::Render
{
  fg::node_id_t mk_backbuffer_acquiring_node();
  fg::node_id_t mk_frame_preparing_node();
  fg::node_id_t mk_gbuffer_main_pass_node();
  fg::node_id_t mk_decals_node();
  fg::node_id_t mk_gbuffer_resolve_node();
  fg::node_id_t mk_early_transparent_sync_node();
  fg::node_id_t mk_transparent_sync_node();
  fg::node_id_t mk_post_process_node();
  fg::node_id_t mk_taa_node();
  fg::node_id_t mk_ui_node();
  fg::node_id_t mk_dbg_text_node();
  fg::node_id_t mk_present_node();
}
