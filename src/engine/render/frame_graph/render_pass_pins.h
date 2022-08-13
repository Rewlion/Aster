#pragma once

#include "handles.h"

#include <EASTL/vector.h>

namespace fg
{
  struct RenderPassPins
  {
    inline VirtualResourceHandle write(const VirtualResourceHandle h)
    {
      return hasWrite(h) ? h : writes.emplace_back(h);
    }

    inline VirtualResourceHandle read(const VirtualResourceHandle h)
    {
      return hasRead(h) ? h : reads.emplace_back(h);
    }

    inline VirtualResourceHandle create(const VirtualResourceHandle h)
    {
      return hasCreate(h) ? h : creates.emplace_back(h);
    }

    inline bool hasWrite(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)writes.size();
    }

    inline bool hasRead(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)reads.size();
    }

    inline bool hasCreate(const VirtualResourceHandle h)
    {
      return h < (VirtualResourceHandle)creates.size();
    }

    inline void setSideEffect()
    {
      sideEffect = true;
    }

    inline bool hasSideEffect()
    {
      return sideEffect;
    }

    bool sideEffect = false;
    eastl::vector<VirtualResourceHandle> writes;
    eastl::vector<VirtualResourceHandle> reads;
    eastl::vector<VirtualResourceHandle> creates;
  };
}