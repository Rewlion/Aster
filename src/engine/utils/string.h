#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

namespace Utils
{
  void remove_spaces(string& str);
  eastl::vector<string> split(const string& str, char delim);
}
