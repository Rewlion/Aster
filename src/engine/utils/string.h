#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

namespace Utils
{
  string remove_spaces(const string& str);
  eastl::vector<string> split(const string& str, char delim);
}
