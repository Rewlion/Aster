#pragma once

#include "registry.h"

namespace fg
{
  void init();
  void destroy();

  auto register_node(const char* name, const char* file, BuildFunction build_cb) -> node_id_t;

  void exec_new_frame();

  void set_closing_node(const char* node);
}
