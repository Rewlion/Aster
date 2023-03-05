#pragma once

#include <engine/types.h>

namespace console
{
  void init();
  void exec(string_view msg);
  void log(string&& msg);
  void logerror(string&& msg);
}
