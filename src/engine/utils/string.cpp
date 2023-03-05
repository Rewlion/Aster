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

  template<class T>
  eastl::vector<T> split_impl(const T& str, char delim)
  {
    auto strs = str | std::views::split(delim)
                    | std::views::filter([](const auto& rng)
                    {
                      return string_view{rng.begin(), rng.end()} != "";
                    });
    eastl::vector<T> res;
    res.reserve(std::distance(strs.begin(), strs.end()));

    for (const auto& strRange: strs)
      res.push_back(T{strRange.begin(), strRange.end()});

    return res;
  }

  eastl::vector<string> split(const string& str, char delim)
  {
    return split_impl<string>(str, delim);
  }

  eastl::vector<string_view> split_view(const string_view str, char delim)
  {
    return split_impl<string_view>(str, delim);
  }
}
