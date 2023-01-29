#include "string.h"

#include <ranges>

namespace Utils
{
  void remove_spaces(string& str)
  {
    str.erase(std::remove_if(
                str.begin(),
                str.end(),
                [](auto c) { return std::isspace(c); }), str.end());
  }

  eastl::vector<string> split(const string& str, char delim)
  {
    eastl::vector<string> res;
    for (const auto& strRange: str | std::views::split(delim))
      res.push_back(string{strRange.begin(), strRange.end()});
    return res;
  }
}
