#pragma once

namespace ed
{
  class Scope;
}

class GIOnSurfels
{
  public:
    GIOnSurfels() = default;
    GIOnSurfels(const ed::Scope*)
    {
      initialized = false;
    }

    bool initialized;
};
