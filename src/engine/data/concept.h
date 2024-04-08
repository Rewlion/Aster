#pragma once

namespace ed
{
  class Scope;
}

template<class T>
concept HasCtorForEngineData = requires (const ed::Scope* data)
{
  T{data};
};
