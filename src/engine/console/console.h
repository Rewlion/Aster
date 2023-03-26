#pragma once

#include <engine/types.h>

namespace console
{
  void init();
  void exec(string_view msg);
  void clog(string&& msg);
  void clogerror(string&& msg);
  void enable(const bool enabled);
}
