#pragma once

#include <EASTL/vector.h>
#include <engine/types.h>

namespace Utils
{
  eastl::vector<char> read_file(const string_view file);
  string read_file_as_string(const string_view& file);
  string get_file_name(const string& filePath);
  string get_file_ext(const string& fileName);
  bool check_file_ext(const string& fileName, const string requiredExt);
  string get_dir_name(const string& filePath);
}
