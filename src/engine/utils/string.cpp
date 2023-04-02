#include "string.h"

#include <ranges>

namespace Utils
{
  auto remove_spaces(const string& str) -> string
  {
    string res = str;
    res.erase(std::remove_if(
                res.begin(),
                res.end(),
                [](auto c) { return std::isspace(c); }), res.end());
    return res;
  }

  template<class T>
  auto split_impl(const T& str, char delim) -> eastl::vector<T>
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

  auto split(const string& str, char delim) -> eastl::vector<string>
  {
    return split_impl<string>(str, delim);
  }

  auto split_view(const string_view str, char delim) -> eastl::vector<string_view>
  {
    return split_impl<string_view>(str, delim);
  }

  void for_each_view_substr(const string_view str,
                            const string_view delim,
                            eastl::function<bool(const string_view)> cb)
  {
    auto strs = str | std::views::split(delim)
                    | std::views::filter([](const auto& rng)
                    {
                      return string_view{rng.begin(), rng.end()} != "";
                    });
    for (auto rng : strs)
    {
      string_view view{rng.begin(), rng.end()};
      if (!cb(view))
        break;
    }
  }

  bool find_view_substr(const string_view str, 
                        const string_view delim, 
                        eastl::function<bool(const string_view)> cb)
  {
    bool found = false;
    for_each_view_substr(str,delim, [&](const string_view view){
      if (cb(view))
      {
        found = true;
        return false;
      }
      return true;
    });

    return found;
  }
}
