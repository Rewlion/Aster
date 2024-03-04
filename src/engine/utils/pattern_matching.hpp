#pragma once

#include <variant>

template<class... Ts>
struct match : Ts...
{
  using Ts::operator()...;
};

template<class... Ts>
match(Ts...) -> match<Ts...>;

template <class... Ts, class... Gs>
constexpr
decltype(auto) operator>> (const std::variant<Ts...>& v,
                           const match<Gs...>& match)
{
  return std::visit(match, v);
}
