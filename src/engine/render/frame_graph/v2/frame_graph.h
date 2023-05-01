#pragma once

#include "registry.h"

namespace fg2
{
  void init();
  void destroy();

  auto register_node(const char* name, const char* file, BuildFunction build_cb) -> NodeHandle;

  void exec_new_frame();

  void set_closing_node(const char* node);
}
