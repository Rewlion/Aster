#pragma once

#include "forward_decl.h"

#include <EASTL/vector_map.h>

namespace qjs
{
  struct RuntimeState
  {
    eastl::vector_map<const char*, JSClassID> classNameToIdsMap;
    void* userState = nullptr;
  };
}
