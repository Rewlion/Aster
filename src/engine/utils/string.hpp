#pragma once

#include <string>

namespace Utils
{
  template<class T>
  void remove_spaces(T& str)
  {
    str.erase(std::remove_if(
                str.begin(),
                str.end(),
                [](auto c) { return std::isspace(c); }), str.end());
  }
}
