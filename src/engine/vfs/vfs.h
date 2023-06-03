#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>

namespace vfs
{
  void init();
  void mount_source(const string_view path, const string_view to);
  eastl::vector<char> read_file(const string_view path);
}
