#pragma once

#include "registry.h"

namespace fg
{
  void init();
  void destroy();

  void register_node(const char* name, const char* file, BuildFunction build_cb);

  void exec_new_frame();

  void set_closing_node(const char* node);
}
