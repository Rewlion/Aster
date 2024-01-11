#pragma once

#include <concepts>

namespace Utils
{
  template<typename T, typename ... U>
  concept IsAnyOf = (std::same_as<T, U> || ...);
}