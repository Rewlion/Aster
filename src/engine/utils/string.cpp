#include "string.h"

#include <ranges>

namespace Utils
{
  string remove_spaces(const string& str)
  {
    string res = str;
    res.erase(std::remove_if(
                res.begin(),
                res.end(),
                [](auto c) { return std::isspace(c); }), res.end());
    return res;
  }

  eastl::vector<string> split(const string& str, char delim)
  {
    eastl::vector<string> res;
    for (const auto& strRange: str | std::views::split(delim))
      res.push_back(string{strRange.begin(), strRange.end()});
    return res;
  }
}
