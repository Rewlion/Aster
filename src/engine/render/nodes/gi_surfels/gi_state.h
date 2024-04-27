#pragma once

namespace ed
{
  class Scope;
}

class GIOnSurfels
{
  public:
    GIOnSurfels() = default;
    GIOnSurfels(const ed::Scope*) {}

    void init();

    bool initialized;
};
