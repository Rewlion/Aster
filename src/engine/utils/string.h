#pragma once

#include <engine/types.h>

#include <EASTL/vector.h>
#include <EASTL/functional.h>

namespace Utils
{
  auto remove_spaces(const string& str) -> string;
  auto split(const string& str, char delim) -> eastl::vector<string>;
  auto split_view(const string_view str, char delim) -> eastl::vector<string_view>;
  void for_each_view_substr(const string_view str, 
                            const string_view delim, 
                            eastl::function<bool(const string_view)>);
  bool find_view_substr(const string_view str, 
                        const string_view delim, 
                        eastl::function<bool(const string_view)>);
}
